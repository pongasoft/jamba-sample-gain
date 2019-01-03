//------------------------------------------------------------------------
// Implementation of the view. In the grand scheme of things, implementing
// this somewhat complex behavior is relatively simple thanks to the fact
// that you can use as many params as you want
//------------------------------------------------------------------------
#include "LinkedSliderView.h"

namespace pongasoft {
namespace VST {
namespace JSGain {
namespace GUI {

/*
 * This is how this view (the left slider in this case) is defined in the XML file.
 * <view background-offset="0, 0" bitmap="slider_background" bitmap-offset="0, 0" class="JSGain::LinkedSlider"
 *       control-tag="Param_LeftGain" default-value="0.7" draw-back="false" draw-back-color="~ BlackCColor"
 *       draw-frame="false" draw-frame-color="~ GreyCColor" draw-value="false" draw-value-color="~ WhiteCColor"
 *       draw-value-from-center="false" draw-value-inverted="false" frame-width="1" handle-bitmap="slider_handle"
 *       handle-offset="0, 2" max-value="1" min-value="0" mode="free click" mouse-enabled="true" opacity="1"
 *       orientation="horizontal" origin="70, 60" reverse-orientation="false" size="130, 18" transparent="false"
 *       transparent-handle="true" wants-focus="true" wheel-inc-value="0.1" zoom-factor="10"/>
 */

//------------------------------------------------------------------------
// LinkedSliderView::setTag - This is called whenever the "control-tag"
// is changed in the editor or when the XML file is loaded => because the
// parameters depend on this value we must call registerParameters
// whenever it changes
//------------------------------------------------------------------------
void LinkedSliderView::setTag(int32_t val)
{
  CSlider::setTag(val);
  registerParameters();
}

//------------------------------------------------------------------------
// LinkedSliderView::registerParameters - this is called by Jamba (and
// from setTag) when the parameters need to be registered
//------------------------------------------------------------------------
void LinkedSliderView::registerParameters()
{
  // both must be set to continue
  if(!fParamCxMgr || getTag() < 0)
    return;

  //------------------------------------------------------------------------
  // Note how registering the fLink param uses the actual parameter defined
  // in JSGainPlugin.h. fParams is made available through
  // PluginCustomViewAdapter<xxx, JSGainGUIState>
  //------------------------------------------------------------------------
  fLink = registerParam(fParams->fLinkParam);

  // which slider is this one? Note how we do not use the enumeration but
  // the actual paramID from the parameter.
  if(getTag() == fParams->fLeftGainParam->fParamID)
  {
    //------------------------------------------------------------------------
    // Passing false as 2nd argument means that we are NOT interested in
    // update for this parameter which makes total sense in this case because
    // fGain represents the value associated to "this" slider.
    // Note how we simply register Raw param.
    //------------------------------------------------------------------------
    // in this case fGain = left, fLinkedGain = right
    fGain = registerRawVstParam(fParams->fLeftGainParam->fParamID, false);
    fLinkedGain = registerRawVstParam(fParams->fRightGainParam->fParamID);
  }
  else
  {
    if(getTag() == fParams->fRightGainParam->fParamID)
    {
      // in this case fGain = right, fLinkedGain = left
      fGain = registerRawVstParam(fParams->fRightGainParam->fParamID, false);
      fLinkedGain = registerRawVstParam(fParams->fLeftGainParam->fParamID);
    }
    else
    {
      // the control-tag has been assigned a parameter that is neither the left
      // or right one => we simply unregister the previously registered params
      // Note that this branch of the code should NOT happen in Release mode!
      fGain = unregisterParam(fGain);
      fLinkedGain = unregisterParam(fLinkedGain);
    }
  }
}

//------------------------------------------------------------------------
// LinkedSliderView::onParameterChange - the callback which is invoked
// when the value of the registered param changes (in this case either
// fLink or fLinkedGain since fGain was registered with "false")
//------------------------------------------------------------------------
void LinkedSliderView::onParameterChange(ParamID iParamID)
{
//------------------------------------------------------------------------
// EDITOR_MODE is a define that exists while in debug/development mode
// Since while in the editor you can freely change values it is possible
// that this would be called with parameters that don't exist. So we
// protect from it. In Release mode, this should obviously not happen.
//------------------------------------------------------------------------
#if EDITOR_MODE
  if(!fLink.exists() || !fGain.exists() || !fLinkedGain.exists())
    return;
#endif

  // case when fLinkedGain has changed
  if(fLinkedGain.getParamID() == iParamID)
  {
    // if link is enabled (note the use of the wrapper class which acts as a bool)
    if(fLink)
    {
      // different value?
      if(fGain != fLinkedGain)
        // simply copy fLinkedGain to fGain => this will trigger the CSlider to refresh
        fGain.copyValueFrom(fLinkedGain);
    }
    return;
  }

  // case when fLink has changed
  if(fLink.getParamID() == iParamID)
  {
    // case when we went from not linked -> linked
    if(fLink)
    {
      // we decide that the one that has the greatest value is the one who "wins" (arbitrary.. could be the other way
      // around)
      if(fGain.getValue() < fLinkedGain.getValue())
      {
        fGain.copyValueFrom(fLinkedGain);
      }
    }
  }
}

//------------------------------------------------------------------------
// This makes the LinkedSliderView class available to the editor (and
// required for loading the XML) => the first parameter is what is
// stored in the XML [class="JSGain::LinkedSlider"], the 2nd one is what
// you see in the editor in the "Views" tab
//------------------------------------------------------------------------
LinkedSliderView::Creator __gLinkedSliderCreator("JSGain::LinkedSlider", "JSGain - LinkedSlider");


//------------------------------------------------------------------------------------------------------------
// For the sake of this example plugin, the LinkedSliderView was created non generic to show how you can tie
// it to the state/params that are defined in JSGainPlugin.h => the wiring (registerParameters) is done in
// the code. The following class demonstrates how it would be possible to make it completely generic by using
// tags/paramIDs instead (get/setTag for "this" slider, get/setLinkTag for the toggle, get/setLinkedSlider for
// the other slider). The wiring is then done using those tags/paramIDs instead of actual params.
//------------------------------------------------------------------------------------------------------------

namespace Generic {

class GenericLinkedSliderView : public CustomViewAdapter<CSlider>
{
public:
  explicit GenericLinkedSliderView(const CRect &iSize) :
    CustomViewAdapter(iSize, nullptr, -1, 0, 0, nullptr, nullptr) {}

  void registerParameters() override;
  void onParameterChange(ParamID iParamID) override;

  void setTag(int32_t val) override { CSlider::setTag(val); registerParameters(); }
  void setLinkTag (int32_t iTag) { fLinkTag = iTag; registerParameters(); }
  int32_t getLinkTag () const { return fLinkTag; }
  void setLinkedSliderTag (int32_t iTag) { fLinkedSliderTag = iTag; registerParameters(); }
  int32_t getLinkedSliderTag () const { return fLinkedSliderTag; }

protected:
  int32_t fLinkTag{-1};
  int32_t fLinkedSliderTag{-1};

  GUIVstParam<bool> fLink{};
  GUIRawVstParam fSlider{};
  GUIRawVstParam fLinkedSlider{};

public:
  class Creator : public TCustomViewCreator<GenericLinkedSliderView>
  {
  public:
    explicit Creator(char const *iViewName = nullptr, char const *iDisplayName = nullptr) noexcept :
      TCustomViewCreator(iViewName, iDisplayName, VSTGUI::UIViewCreator::kCSlider)
    {
      //------------------------------------------------------------------------
      // 2 attributes are needed: one for link-tag and one for linked-slider-tag
      //------------------------------------------------------------------------
      registerTagAttribute("link-tag", &GenericLinkedSliderView::getLinkTag, &GenericLinkedSliderView::setLinkTag);
      registerTagAttribute("linked-slider-tag", &GenericLinkedSliderView::getLinkedSliderTag, &GenericLinkedSliderView::setLinkedSliderTag);
    }
  };
};

//------------------------------------------------------------------------
// GenericLinkedSliderView::registerParameters - use the 3 tags
//------------------------------------------------------------------------
void GenericLinkedSliderView::registerParameters()
{
  // all must be set to continue
  if(!fParamCxMgr || getTag() < 0 || getLinkTag() < 0 || getLinkedSliderTag() < 0)
    return;

  if(getTag() == getLinkedSliderTag())
  {
    DLOG_F(ERROR, "this slider and the linked slider must have different tags!");
    fLink = unregisterParam(fLink);
    fSlider = unregisterParam(fSlider);
    fLinkedSlider = unregisterParam(fLinkedSlider);
  }
  else
  {
    fLink = registerVstParam<bool>(static_cast<ParamID>(getLinkTag()));
    fSlider = registerRawVstParam(static_cast<ParamID>(getTag()), false);
    fLinkedSlider = registerRawVstParam(static_cast<ParamID>(getLinkedSliderTag()));
  }
}

//------------------------------------------------------------------------
// GenericLinkedSliderView::onParameterChange - this is the same code
// as the non generic version!
//------------------------------------------------------------------------
void GenericLinkedSliderView::onParameterChange(ParamID iParamID)
{
#if EDITOR_MODE
  if(!fLink.exists() || !fSlider.exists() || !fLinkedSlider.exists())
    return;
#endif

  if(fLinkedSlider.getParamID() == iParamID)
  {
    if(fLink && fSlider != fLinkedSlider)
      fSlider.copyValueFrom(fLinkedSlider);
    return;
  }

  if(fLink.getParamID() == iParamID)
  {
    if(fLink && fSlider.getValue() < fLinkedSlider.getValue())
      fSlider.copyValueFrom(fLinkedSlider);
    return;
  }
}

// Uncomment to make available
// GenericLinkedSliderView::Creator __gLinkedSliderCreator("JSGain::GenericLinkedSlider", "JSGain - GenericLinkedSlider");
}

}
}
}
}
