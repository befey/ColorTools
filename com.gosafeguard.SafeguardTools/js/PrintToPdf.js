var csInterface = new CSInterface(); 

// Create events for the button presses
var makePdfEvent = new CSEvent("com.gosafeguard.SafeguardTools.PrintToPdf.makepdfbutton", "APPLICATION", "ILST", "PrintToPdf");

var cancelEvent = new CSEvent("com.gosafeguard.SafeguardTools.PrintToPdf.cancelbutton", "APPLICATION", "ILST", "PrintToPdf");
cancelEvent.data = "Cancel Button Pressed";

$(function()
{
	//csInterface.setWindowTitle("Print To PDF");
	
	$('#allpages-check').change(function()
	{
   		$("#range-text").prop("disabled", $(this).is(':checked'));
   		
   		if ($(this).is(':checked'))
   		{
   			$("#range-text").css("color","gray");
   		}
   		else
   		{
   			$("#range-text").focus();
   			$("#range-text").css("color","black");
   		}
	});
});


function sendDataToIllustrator()
{
	
	var data = {
		"preset-select"		:	parseInt($("#preset-select").val(), 10),
		"range-text"		:	$("#range-text").val(),
		"allpages-check"	:	$("#allpages-check").is(':checked')
	};
	makePdfEvent.data = JSON.stringify(data);
	csInterface.dispatchEvent(makePdfEvent);
}