var csInterface = new CSInterface(); 

// Create events for the button presses
var makePdfEvent = new CSEvent("com.gosafeguard.SafeguardTools.PrintToPdf.makepdfbutton", "APPLICATION", "ILST", "PrintToPdf");

var cancelEvent = new CSEvent("com.gosafeguard.SafeguardTools.PrintToPdf.cancelbutton", "APPLICATION", "ILST", "PrintToPdf");

var resultsBackEvent = new CSEvent("com.gosafeguard.SafeguardTools.PrintToPdf.resultsback", "APPLICATION", "ILST", "PrintToPdf");

$(function()
{
	csInterface.setWindowTitle("Print To PDF");
	
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
	
	csInterface.addEventListener("com.gosafeguard.SafeguardTools.PrintToPdf.resultsback", onResultsBack);
	csInterface.addEventListener("com.gosafeguard.SafeguardTools.PrintToPdf.clearresultbox", clearResultBox);
	csInterface.addEventListener("com.gosafeguard.SafeguardTools.PrintToPdf.forcepanelclose", 
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

function onResultsBack(event)
{
	var xmlData = $.parseXML(event.data);
	var $xml = $(xmlData);
	
	$xml.find("delete").each( function(index)
	{
		$("#results-textarea").append("<div class='deleted'> - " + this.textContent + "</div><br />"); //.addClass("deleted");
	});
	$("#results-textarea").append("-----<br />")
	$xml.find("create").each( function(index)
	{
		$("#results-textarea").append("<div class='created'> + " + this.textContent + "</div><br />"); //.addClass("created");
	});
	
	$("#cancel-button").focus();
}

function clearResultBox(event)
{
	$("#results-textarea").text('');
}