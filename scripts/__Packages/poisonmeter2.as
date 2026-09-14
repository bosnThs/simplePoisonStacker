class PoisonMeter extends MovieClip
{
	var BottomLeftLockInstance;
	var BottomRightLockInstance;
    var LeftChargeMeter;
    var RightChargeMeter;

	function PoisonMeter()
	{
		super();
		this.LeftChargeMeter = new Components.Meter(this.BottomLeftLockInstance.LeftHandPoisonMeterInstance, false);
		this.RightChargeMeter = new Components.Meter(this.BottomRightLockInstance.RightHandPoisonMeterInstance, true);
	}

	function onLoad()
	{
		this.doHide();
	}

	function doHide()
	{
		this._visible = false;
	}
	function doShow()
	{
		this._visible = true;
	}
	
	function setPosition(xpos, ypos, rot, xscale, yscale, abarColor, aborderColor, abgColor, abLeftHand, aFadeOut, aCurrentTime)
	{
		var _loc3_ = !abLeftHand ? this.RightChargeMeter : this.LeftChargeMeter;
		_loc3_.setPosition(xpos, ypos, rot, xscale, yscale, abarColor, aborderColor, abgColor, aFadeOut, aCurrentTime)   
	}
	
   
	function setMeterPercent(aPercent, abLeftHand, aCurrentTime, barGrowth)
	{
      var _loc3_ = !abLeftHand ? this.RightChargeMeter : this.LeftChargeMeter;
	 _loc3_.setMeterPercent(aPercent, aCurrentTime, barGrowth);	  
	}
}