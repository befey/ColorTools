var csInterface = new CSInterface();

// Create events for the button presses
var okEvent = new CSEvent("com.gosafeguard.SafeguardTools.PrintToPdf.FolderPrefs.okbutton", "APPLICATION", "ILST", "PrintToPdf");

var cancelEvent = new CSEvent("com.gosafeguard.SafeguardTools.PrintToPdf.FolderPrefs.cancelbutton", "APPLICATION", "ILST", "PrintToPdf");

var panelLoadedEvent = new CSEvent("com.gosafeguard.SafeguardTools.PrintToPdf.FolderPrefs.panelloaded", "APPLICATION", "ILST", "PrintToPdf");

$(function()
  {
  csInterface.setWindowTitle("Set output folder preferences:");
  
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
    var xmlData = $.parseXML(event.data);
    var $xml = $(xmlData);
    
    $xml.find("folder").each( function(index, element)
                             {
                             $("#folderprefs-textarea").html(function()
                                                           {
                                                           var newHtml = $("#folderprefs-textarea").html();
                                                           
                                                           newHtml += "<div class='folder'>";
                                                           newHtml += $(element).find("preset-name").text();
                                                           newHtml += "<br />";
                                                           newHtml += $(element).find("path").text();
                                                           newHtml += "</div>";
                                                           return newHtml;
                                                           });
                             });
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
