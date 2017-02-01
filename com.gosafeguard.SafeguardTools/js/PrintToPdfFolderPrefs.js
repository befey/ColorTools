var csInterface = new CSInterface();

// Create events for the button presses
var okEvent = new CSEvent("com.gosafeguard.SafeguardTools.PrintToPdf.FolderPrefs.okbutton", "APPLICATION", "ILST", "PrintToPdf");

var cancelEvent = new CSEvent("com.gosafeguard.SafeguardTools.PrintToPdf.FolderPrefs.cancelbutton", "APPLICATION", "ILST", "PrintToPdf");

var panelLoadedEvent = new CSEvent("com.gosafeguard.SafeguardTools.PrintToPdf.FolderPrefs.panelloaded", "APPLICATION", "ILST", "PrintToPdf");

$(function()
  {
  csInterface.setWindowTitle("Set output folder preferences:");
  
  csInterface.addEventListener("com.gosafeguard.SafeguardTools.PrintToPdf.FolderPrefs.resultsback", onResultsBack);
  csInterface.addEventListener("com.gosafeguard.SafeguardTools.PrintToPdf.FolderPrefs.forcepanelclose",
                               function(event)
                               {
                               csInterface.closeExtension();
                               });
  csInterface.addEventListener("com.gosafeguard.SafeguardTools.PrintToPdf.FolderPrefs.datafromplugin", ReceiveDataFromPlugin);
  
  csInterface.dispatchEvent(panelLoadedEvent);
  }
  );

function ReceiveDataFromPlugin(event)
{
    jsonArtboardData = event.data;
    
    jsonArtboardData.dto.plates.forEach(function(element, index, array)
                                        {
                                        storedArtboardPrint[index] = element.shouldPrint;
                                        });
    
    PutColorList(jsonArtboardData);
}

function SendFoldersToPlugin()
{
    
    var data = {
        "preset-select"			:	parseInt($("#preset-select").val(), 10),
        "separatefiles-check"	:	$("#separatefiles-check").is(':checked'),
        "useroutputfolder-check":	$("#useroutputfolder-check").is(':checked'),
        "donotdelete-check"     :	$("#donotdelete-check").is(':checked'),
        "dto"                   :   JSON.stringify(jsonArtboardData)
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
