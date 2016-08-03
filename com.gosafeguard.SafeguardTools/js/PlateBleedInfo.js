var csInterface = new CSInterface(); 

// Create events for the button presses
var cancelEvent = new CSEvent("com.gosafeguard.SafeguardTools.PlateBleedInfo.cancelbutton", "APPLICATION", "ILST", "PlateBleedInfo");

$(function()
{
	csInterface.setWindowTitle("Edit Plate Bleed Info");

	csInterface.addEventListener("com.gosafeguard.SafeguardTools.PlateBleedInfo.forcepanelclose", 
	function(event)
	{
		csInterface.closeExtension();
	});
});


function sendDataToIllustrator()
{
	var data = {
		"preset-select"			:	parseInt($("#preset-select").val(), 10),
		"range-text"			:	$("#range-text").val(),
		"allpages-check"		:	$("#allpages-check").is(':checked'),
		"separatefiles-check"	:	$("#separatefiles-check").is(':checked')
	};
	makePdfEvent.data = JSON.stringify(data);
	csInterface.dispatchEvent(makePdfEvent);
}