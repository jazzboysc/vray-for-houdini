//
// Copyright (c) 2015, Chaos Software Ltd
//
// V-Ray For Houdini
//
// Andrei Izrantcev <andrei.izrantcev@chaosgroup.com>
//
// All rights reserved. These coded instructions, statements and
// computer programs contain unpublished information proprietary to
// Chaos Software Ltd, which is protected by the appropriate copyright
// laws and may not be disclosed to third parties or copied or
// duplicated, in whole or in part, without prior written consent of
// Chaos Software Ltd.
//

#include <boost/format.hpp>

#include "vfh_defines.h"
#include "vfh_typedefs.h"
#include "vfh_tex_utils.h"


#define CGR_DEBUG_RAMPS  0
#define CGR_MAX_NUM_POINTS 64


struct MyPoint {
	float x;
	float y;
};


void VRayForHoudini::Texture::exportRampAttribute(VRayExporter *exporter, Attrs::PluginDesc &pluginDesc, OP_Node *op_node,
												  const std::string &rampAttrName,
												  const std::string &colAttrName, const std::string &posAttrName, const std::string &typesAttrName, const bool asColor)
{
	const fpreal &t = exporter->getContext().getTime();

	const std::string &pluginName = Attrs::PluginDesc::GetPluginName(op_node);

	int nPoints = op_node->evalInt(rampAttrName.c_str(), 0, 0.0f);

#if CGR_DEBUG_RAMPS
	PRINT_INFO("Ramp points: %i",
			   nPoints);
#endif

	const bool needTypes = NOT(typesAttrName.empty());

	const std::string &prmPosName    = boost::str(boost::format("%s#pos")    % rampAttrName);
	const std::string &prmColName    = boost::str(boost::format("%s#c")      % rampAttrName);
	const std::string &prmInterpName = boost::str(boost::format("%s#interp") % rampAttrName);

	VRay::ValueList colorPlugins;
	VRay::ColorList colorList;
	VRay::FloatList positions;
	VRay::IntList   types;

	for(int i = 1; i <= nPoints; i++) {
		const float pos = (float)op_node->evalFloatInst(prmPosName.c_str(), &i, 0, t);

		const float colR = (float)op_node->evalFloatInst(prmColName.c_str(), &i, 0, t);
		const float colG = (float)op_node->evalFloatInst(prmColName.c_str(), &i, 1, t);
		const float colB = (float)op_node->evalFloatInst(prmColName.c_str(), &i, 2, t);

		const int interp = op_node->evalIntInst(prmInterpName.c_str(), &i, 0, t);
#if CGR_DEBUG_RAMPS
		PRINT_INFO(" %.3f: Color(%.3f,%.3f,%.3f) [%i]",
				   pos, colR, colG, colB, interp);
#endif
		const std::string &colPluginName = boost::str(boost::format("%sPos%i") % pluginName % i);

		if (asColor) {
			colorList.push_back(VRay::Color(colR, colG, colB));
			positions.push_back(pos);
			if (needTypes) {
				types.push_back(interp);
			}
		}
		else {
			Attrs::PluginDesc colPluginDesc(colPluginName, "TexAColor");
			colPluginDesc.pluginAttrs.push_back(Attrs::PluginAttr("texture", Attrs::PluginAttr::AttrTypeAColor, colR, colG, colB, 1.0f));

			VRay::Plugin colPlugin = exporter->exportPlugin(colPluginDesc);
			if (colPlugin) {
				colorPlugins.push_back(VRay::Value(colPlugin));
				positions.push_back(pos);
				if (needTypes) {
					// TODO: Remap interp value to V-Ray's enum
#if 0
					switch(ramp.interpolation()) {
						case BL::ColorRamp::interpolation_CONSTANT: interp = 0; break;
						case BL::ColorRamp::interpolation_LINEAR:   interp = 1; break;
						case BL::ColorRamp::interpolation_EASE:     interp = 2; break;
						case BL::ColorRamp::interpolation_CARDINAL: interp = 3; break;
						case BL::ColorRamp::interpolation_B_SPLINE: interp = 4; break;
						default:                                    interp = 1;
					}
#endif
					types.push_back(interp);
				}
			}
		}
	}

	if (asColor) {
		pluginDesc.pluginAttrs.push_back(Attrs::PluginAttr(colAttrName, colorList));
	}
	else {
		pluginDesc.pluginAttrs.push_back(Attrs::PluginAttr(colAttrName, colorPlugins));
	}
	pluginDesc.pluginAttrs.push_back(Attrs::PluginAttr(posAttrName, positions));
	if (needTypes) {
		pluginDesc.pluginAttrs.push_back(Attrs::PluginAttr(typesAttrName, types));
	}
}


void VRayForHoudini::Texture::getCurveData(VRayExporter *exporter, OP_Node *op_node,
										   const std::string &curveAttrName,
										   VRay::IntList &interpolations, VRay::FloatList &positions, VRay::FloatList *values,
										   const bool needHandles)
{
	const fpreal &t = exporter->getContext().getTime();

	int numPoints = op_node->evalInt(curveAttrName.c_str(), 0, t);
	if (NOT(numPoints))
		return;

	MyPoint point[CGR_MAX_NUM_POINTS];

	const std::string &prmPosName    = boost::str(boost::format("%s#pos")    % curveAttrName);
	const std::string &prmValName    = boost::str(boost::format("%s#value")  % curveAttrName);
	const std::string &prmInterpName = boost::str(boost::format("%s#interp") % curveAttrName);

	int p = 0;
	for(int i = 1; i <= numPoints; ++i, ++p) {
		const float pos    = (float)op_node->evalFloatInst(prmPosName.c_str(),    &i, 0, t);
		const float val    = (float)op_node->evalFloatInst(prmValName.c_str(),    &i, 0, t);
		const float interp = (float)op_node->evalFloatInst(prmInterpName.c_str(), &i, 0, t);

		if (NOT(needHandles)) {
			positions.push_back(pos);
			if (values) values->push_back(val); else positions.push_back(val);
		}
		else {
			point[p].x = pos;
			point[p].y = val;
		}

		// TODO: Remap "interp" to V-Ray enum
		interpolations.push_back(interp);
	}

	if (NOT(needHandles)) {
		return;
	}

	float  deltaX[CGR_MAX_NUM_POINTS + 1];
	float  ySecon[CGR_MAX_NUM_POINTS];
	float  yPrim[CGR_MAX_NUM_POINTS];
	float  d[CGR_MAX_NUM_POINTS];
	float  w[CGR_MAX_NUM_POINTS];
	int    i;

	for(i = 1; i < numPoints; i++)
		deltaX[i] = point[i].x - point[i-1].x;
	deltaX[0] = deltaX[1];
	deltaX[numPoints] = deltaX[numPoints-1];
	for(i = 1; i < numPoints-1; i++) {
		d[i] = 2 * (point[i + 1].x - point[i - 1].x);
		w[i] = 6 * ((point[i + 1].y - point[i].y) / deltaX[i+1] - (point[i].y - point[i - 1].y) / deltaX[i]);
	}
	for(i = 1; i < numPoints-2; i++) {
		w[i + 1] -= w[i] * deltaX[i+1] / d[i];
		d[i + 1] -= deltaX[i+1] * deltaX[i+1] / d[i];
	}
	ySecon[0] = 0;
	ySecon[numPoints-1] = 0;
	for(i = numPoints - 2; i >= 1; i--)
		ySecon[i] = (w[i] - deltaX[i+1] * ySecon[i + 1]) / d[i];
	for(i = 0; i < numPoints-1; i++)
		yPrim[i] = (point[i+1].y - point[i].y) / deltaX[i+1] - (deltaX[i+1] / 6.0f) * (2 * ySecon[i] + ySecon[i+1]);
	yPrim[i] = (point[i].y - point[i-1].y) / deltaX[i] + (deltaX[i] / 6.0f) * ySecon[i-1];

	for(p = 0; p < numPoints; ++p) {
		const float &px = point[p].x;
		const float &py = point[p].y;

		const float h1x = -deltaX[p] / 3;
		const float h1y = -deltaX[p] * yPrim[p] / 3;

		const float h2x = deltaX[p+1] / 3;
		const float h2y = deltaX[p+1] * yPrim[p] / 3;

		positions.push_back(px);
		if (values) values->push_back(py); else positions.push_back(py);

		positions.push_back(h1x);
		if (values) values->push_back(h1y); else positions.push_back(h1y);

		positions.push_back(h2x);
		if (values) values->push_back(h2y); else positions.push_back(h2y);
	}
}