var csInterface = new CSInterface();

// Create events for the button presses
var okEvent = new CSEvent("com.gosafeguard.SafeguardTools.PrintToPdf.FolderPrefs.okbutton", "APPLICATION", "ILST", "PrintToPdf");

var cancelEvent = new CSEvent("com.gosafeguard.SafeguardTools.PrintToPdf.FolderPrefs.cancelbutton", "APPLICATION", "ILST", "PrintToPdf");

var panelLoadedEvent = new CSEvent("com.gosafeguard.SafeguardTools.PrintToPdf.FolderPrefs.panelloaded", "APPLICATION", "ILST", "PrintToPdf");

var changeFolderEvent = new CSEvent("com.gosafeguard.SafeguardTools.PrintToPdf.FolderPrefs.changefolder", "APPLICATION", "ILST", "PrintToPdf");

$(function()
  {
  csInterface.setWindowTitle("Set output folder preferences:");
  
  csInterface.addEventListener("com.gosafeguard.SafeguardTools.PrintToPdf.FolderPrefs.forcepanelclose",
                               function(event)
                               {
                               csInterface.closeExtension();
                               });
  csInterface.addEventListener("com.gosafeguard.SafeguardTools.PrintToPdf.FolderPrefs.datafromplugin", ReceiveDataFromPlugin);
  csInterface.addEventListener("com.gosafeguard.SafeguardTools.PrintToPdf.FolderPrefs.folderfromplugin", FolderFromPlugin);
  
  $("#folderprefs-textarea").on( "click", ".folder", function()
                              {
                              ChangeFolderPref($(this));
                              });
  
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
                                                           
                                                           newHtml += "<div class='feedback-box folder'>";
                                                           newHtml += "<div class='preset-name'>" + $(element).find("preset-name").text() + "</div>";
                                                           newHtml += "<div class='path'>" + $(element).find("path").text() + "</div>";
                                                           newHtml += "</div>";
                                                           return newHtml;
                                                           });
                             });
}

function FolderFromPlugin(event)
{
    var xmlData = $.parseXML(event.data);
    var $xml = $(xmlData);
    var presetname = $xml.find("folder").find("preset-name").text();
    var path = $xml.find("folder").find("path").text();
    
    $("#folderprefs-textarea").find(".preset-name").each( function(index, element)
                                                         {
                                                            if ($(element).text() == presetname)
                                                            {
                                                                $(element).siblings(".path").html(path);
                                                            }
                                                         });
}


function SendFoldersToPlugin()
{
    var data = [];
 
    $("#folderprefs-textarea").find(".folder").each( function(index, element)
                                                    {
                                                    data.push( [ $(element).find(".preset-name").text(), $(element).find(".path").text() ] );
                                                    });
    okEvent.data = JSON.stringify(data);
    csInterface.dispatchEvent(okEvent);
}

function ChangeFolderPref(folderDiv)
{
    var data = {
        "preset-name" : folderDiv.find(".preset-name").text(),
        "path" : folderDiv.find(".path").text()
    };
    changeFolderEvent.data = JSON.stringify(data);
    csInterface.dispatchEvent(changeFolderEvent);
}
