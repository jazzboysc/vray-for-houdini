//
// Copyright (c) 2015-2016, Chaos Software Ltd
//
// V-Ray For Houdini
//
// ACCESSIBLE SOURCE CODE WITHOUT DISTRIBUTION OF MODIFICATION LICENSE
//
// Full license text: https://github.com/ChaosGroup/vray-for-houdini/blob/master/LICENSE
//

#ifndef VRAY_FOR_HOUDINI_VOP_NODE_MTLMULTI_H
#define VRAY_FOR_HOUDINI_VOP_NODE_MTLMULTI_H

#include "vop_node_base.h"
#include "vfh_prm_templates.h"


namespace VRayForHoudini {
namespace VOP {


class MtlMulti:
		public VOP::NodeBase
{
public:
	static void           addPrmTemplate(Parm::PRMList &prmTemplate);

public:
	MtlMulti(OP_Network *parent, const char *name, OP_Operator *entry):NodeBase(parent, name, entry) {}
	virtual              ~MtlMulti() {}

	// From OP::VRayNode
	virtual PluginResult  asPluginDesc(Attrs::PluginDesc &pluginDesc, VRayExporter &exporter, ExportContext *parentContext=nullptr) VRAY_OVERRIDE;

protected:
	virtual void          setPluginType() VRAY_OVERRIDE;

	// From VOP_Node
public:
	virtual void          getCode(UT_String &codestr, const VOP_CodeGenContext &context) VRAY_OVERRIDE;
	virtual const char   *inputLabel(unsigned idx) const VRAY_OVERRIDE;
	virtual unsigned      getNumVisibleInputs() const VRAY_OVERRIDE;
	virtual unsigned      orderedInputs() const VRAY_OVERRIDE;

protected:
	virtual int           getInputFromName(const UT_String &in) const VRAY_OVERRIDE;
	virtual void          getInputNameSubclass(UT_String &in, int idx) const VRAY_OVERRIDE;
	virtual int           getInputFromNameSubclass(const UT_String &in) const VRAY_OVERRIDE;
	virtual void          getInputTypeInfoSubclass(VOP_TypeInfo &type_info, int idx) VRAY_OVERRIDE;
	virtual void          getAllowedInputTypeInfosSubclass(unsigned idx, VOP_VopTypeInfoArray &type_infos) VRAY_OVERRIDE;

private:
	int                   customInputsCount() const;

};

} // namespace VOP
} // namespace VRayForHoudini

#endif // VRAY_FOR_HOUDINI_VOP_NODE_MTLMULTI_H
