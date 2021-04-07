var csInterface = new CSInterface(); 

// Create events for the button presses
var changeEvent = new CSEvent("com.gosafeguard.SafeguardTools.ColorTools.changebutton", "APPLICATION", "ILST", "ColorTools");

var removeEvent = new CSEvent("com.gosafeguard.SafeguardTools.ColorTools.removebutton", "APPLICATION", "ILST", "ColorTools");

var updateListEvent = new CSEvent("com.gosafeguard.SafeguardTools.ColorTools.updatelist", "APPLICATION", "ILST", "ColorTools");

$(function()
{
	csInterface.setWindowTitle("Find and Replace Graphics");
	
	// Update the color of the panel when the theme color of the product changed..
	updateThemeWithAppSkinInfo(csInterface.hostEnvironment.appSkinInfo);
	csInterface.addEventListener(CSInterface.THEME_COLOR_CHANGED_EVENT, onAppThemeColorChanged);
	
	csInterface.addEventListener("com.gosafeguard.SafeguardTools.ColorTools.updatepanel",
	function(event)
	{
		var xmlData = $.parseXML(event.data);
		var $xml = $(xmlData);
		var gridType = $xml.find('gridtype').text();
	});
	
	csInterface.addEventListener("com.gosafeguard.SafeguardTools.ColorTools.updatelistback", onUpdateListBack);
	csInterface.addEventListener("com.gosafeguard.SafeguardTools.ColorTools.changecountback", onChangeCountBack);
	csInterface.addEventListener("com.gosafeguard.SafeguardTools.ColorTools.changeinback", onChangeInBack);
	csInterface.addEventListener("com.gosafeguard.SafeguardTools.ColorTools.forcepanelclose", 
	function(event)
	{
		csInterface.closeExtension();
	});
	
	setIncludeTintsCheckbox();	
	
});
    

/**
* Update the theme with the AppSkinInfo retrieved from the host product.
*/
function updateThemeWithAppSkinInfo(appSkinInfo) {
	//Update the background color of the panel
	var panelBackgroundColor = appSkinInfo.panelBackgroundColor.color;
	document.body.bgColor = toHex(panelBackgroundColor);
        
	var styleId = 'ppstyle';
	addRule(styleId, '.default', 'font-size:' + appSkinInfo.baseFontSize + 'px' + '; color:' + reverseColor(panelBackgroundColor) + '; background-color:' + toHex(panelBackgroundColor, 20));
	addRule(styleId, 'button', 'border-color: ' + toHex(panelBackgroundColor, -50));
}

function addRule(stylesheetId, selector, rule) {
	var stylesheet = document.getElementById(stylesheetId);
    
	if (stylesheet) {
		stylesheet = stylesheet.sheet;
		if( stylesheet.addRule ){
			stylesheet.addRule(selector, rule);
		} else if( stylesheet.insertRule ) {
			stylesheet.insertRule(selector + ' { ' + rule + ' }', stylesheet.cssRules.length);
		}
	}
}

function reverseColor(color, delta) {
	return toHex({red:Math.abs(255-color.red), green:Math.abs(255-color.green), blue:Math.abs(255-color.blue)}, delta);
}

/**
* Convert the Color object to string in hexadecimal format;
*/
function toHex(color, delta) {
	function computeValue(value, delta) {
		var computedValue = !isNaN(delta) ? value + delta : value;
		if (computedValue < 0) {
			computedValue = 0;
		} else if (computedValue > 255) {
			computedValue = 255;
		}

		computedValue = computedValue.toString(16);
		return computedValue.length == 1 ? '0' + computedValue : computedValue;
	}

	var hex = '';
	if (color) {
		with (color) {
			hex = computeValue(red, delta) + computeValue(green, delta) + computeValue(blue, delta);
		}
	}
	return '#' + hex;
}

function onAppThemeColorChanged(event) {
	// Should get a latest HostEnvironment object from application.
	var skinInfo = JSON.parse(window.__adobe_cep__.getHostEnvironment()).appSkinInfo;
	// Gets the style information such as color info from the skinInfo, 
	// and redraw all UI controls of your extension according to the style info.
	updateThemeWithAppSkinInfo(skinInfo);
} 

function evalScript(script, callback) {
	new CSInterface().evalScript(script, callback);
}

function onUpdateListBack(event)
{
	var xmlData = $.parseXML(event.data);
	var $xml = $(xmlData);
	
	fromSelectedText = $("#from-select option:selected").text();
	toSelectedText = $("#to-select option:selected").text();
	
	$("#from-select").find('option').remove();
	$("#to-select").find('option').remove();
	$xml.find("color").each( function(index)
	{
		if (this.textContent != "[None]" &&
			//this.textContent != "[Registration]" &&
			this.textContent != "GRIPPER")
		{
			$("#from-select").append($("<option/>",
			{
				text: this.textContent
			}));
			$("#to-select").append($("<option/>",
			{
				text: this.textContent
			}));
		}
	});
	
	if ( $("#from-select option").filter(
		function()
		{
			return this.value === fromSelectedText;
		}
		).length !== 0 )
	{
		$("#from-select").val(fromSelectedText);
	}
	else
	{
		$("#from-select").val("Black");
	}
	
	if ( $("#to-select option").filter(
		function()
		{
			return this.value === toSelectedText;
		}
		).length !== 0 )
	{
		$("#to-select").val(toSelectedText);
	}
	else
	{
		$("#to-select").val("Black");
	}
}

function onChangeCountBack(event)
{
	$("#changedCount").text(event.data);
	csInterface.dispatchEvent(updateListEvent);
}

function onChangeInBack(event)
{
	$("#changein-select").val(event.data);
}

function attributeChanged()
{
	if ( $("#attribute-select").val() == 0 )
	{
		$("#addremove-span").toggle(false);
		$("#to-span").toggle(true);
	}
	else
	{
		$("#addremove-span").toggle(true);
		$("#to-span").toggle(false);
	}
	
	setIncludeTintsCheckbox();
}

function addremoveChanged()
{
	setIncludeTintsCheckbox();
}

function fromToChanged()
{
	setIncludeTintsCheckbox();
}

function setIncludeTintsCheckbox()
{
	//This is a neat little expression which turns the Include Tints checkbox on and off depending on whether Overprint is selected
	// and whether Add or Remove is selected
	// We take the Active Entry in Attribute  ( 0 == Color  , 1 == Overprint)
	// We take the Active Entry in Add/Remove ( 0 == Remove , 1 == Add )
	// We only turn the check box OFF if we are ADD OVERPRINT otherwise we turn it on
	$("#tints-checkbox").prop('checked', (!(!!parseInt($("#attribute-select").val(), 10) && !parseInt($("#addremove-select").val(), 10)) ));
	
	//Set tints checkbox off if From/To color contains %
	if ( ($("#from-select").val().indexOf("%") >= 0) ||
		 ($("#to-select").val().indexOf("%") >= 0) )
	{
		$("#tints-checkbox").prop('checked', false);
	}
}

function sendDataToIllustrator()
{
	var data = {
		"attribute-select"	:	parseInt($("#attribute-select").val(), 10),
		"changein-select"	:	parseInt($("#changein-select").val(), 10),
		"from-select"		:	$("#from-select").val(),
		"to-select"			:	$("#to-select").val(),
		"addremove-select"	:	parseInt($("#addremove-select").val(), 10),
		"applyto-select"	:	parseInt($("#applyto-select").val(), 10),
		"tints-checkbox"	:	$("#tints-checkbox").is(':checked')
	};
	changeEvent.data = JSON.stringify(data);
	csInterface.dispatchEvent(changeEvent);
}