#ifndef ISCANINTERFACE_H
#define ISCANINTERFACE_H

#include "iplugininterface.h"

class IScanInterface : public IPluginInterface
{
public:
  IScanInterface();
};
#define IScanInterface_iid "org.smelecomp.IScanInterface/0.1.0"
Q_DECLARE_INTERFACE(IScanInterface, IScanInterface_iid) // ISCANINTERFACE_H

#endif // ISCANINTERFACE_H
