#include "VoicemeeterRemote.h"

class VoicemeeterClient
{
public:
    explicit VoicemeeterClient();
    virtual ~VoicemeeterClient();

    float getMuted();

private:
    void initialize();

    T_VBVMR_INTERFACE iVMR;
};