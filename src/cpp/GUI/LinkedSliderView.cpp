#include "LinkedSliderView.h"

namespace pongasoft {
namespace VST {
namespace JSGain {
namespace GUI {

//------------------------------------------------------------------------
// LinkedSliderView::setTag
//------------------------------------------------------------------------
void LinkedSliderView::setTag(int32_t val)
{
  CSlider::setTag(val);
  registerParameters();
}

//------------------------------------------------------------------------
// LinkedSliderView::registerParameters
//------------------------------------------------------------------------
void LinkedSliderView::registerParameters()
{
  if(!fParamCxMgr || getTag() < 0)
    return;

  fLink = registerVstParam(fParams->fLinkParam);
  if(getTag() == fParams->fLeftGainParam->fParamID)
  {
    fGain = registerRawVstParam(fParams->fLeftGainParam->fParamID, false);
    fLinkedGain = registerRawVstParam(fParams->fRightGainParam->fParamID);
  }
  else
  {
    if(getTag() == fParams->fRightGainParam->fParamID)
    {
      fGain = registerRawVstParam(fParams->fRightGainParam->fParamID, false);
      fLinkedGain = registerRawVstParam(fParams->fLeftGainParam->fParamID);
    }
    else
    {
      fGain = unregisterParam(fGain);
      fLinkedGain = unregisterParam(fLinkedGain);
    }
  }
}

//------------------------------------------------------------------------
// LinkedSliderView::onParameterChange
//------------------------------------------------------------------------
void LinkedSliderView::onParameterChange(ParamID iParamID)
{
#if EDITOR_MODE
  if(!fLink.exists() || !fGain.exists() || !fLinkedGain.exists())
    return;
#endif

  if(fLinkedGain.getParamID() == iParamID)
  {
    if(fLink)
    {
      if(fGain != fLinkedGain)
        fGain.copyValueFrom(fLinkedGain);
    }
    return;
  }

  if(fLink.getParamID() == iParamID)
  {
    // case when we went from not linked -> linked
    if(fLink)
    {
      if(fGain.getValue() < fLinkedGain.getValue())
      {
        fGain.copyValueFrom(fLinkedGain);
      }
    }
  }
}

LinkedSliderView::Creator __gLinkedSliderCreator("JSGain::LinkedSlider", "JSGain - LinkedSlider");

}
}
}
}
