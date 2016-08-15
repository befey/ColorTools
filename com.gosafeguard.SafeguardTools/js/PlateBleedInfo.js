var csInterface = new CSInterface(); 
var jsonArtboardData;
var currArtboardId = 0;

// Create events for the button presses
var panelLoadedEvent = new CSEvent("com.gosafeguard.SafeguardTools.PlateBleedInfo.panelloaded", "APPLICATION", "ILST", "PlateBleedInfo");
var changeArtboardEvent = new CSEvent("com.gosafeguard.SafeguardTools.PlateBleedInfo.changeartboard", "APPLICATION", "ILST", "PlateBleedInfo");
var cancelEvent = new CSEvent("com.gosafeguard.SafeguardTools.PlateBleedInfo.cancelbutton", "APPLICATION", "ILST", "PlateBleedInfo");



$(function()
  {
  csInterface.setWindowTitle("Edit Plate Bleed Info");
  
  csInterface.addEventListener("com.gosafeguard.SafeguardTools.PlateBleedInfo.forcepanelclose",
                               function(event)
                               {
                               csInterface.closeExtension();
                               });
  
  csInterface.addEventListener("com.gosafeguard.SafeguardTools.PlateBleedInfo.datafromplugin", ReceiveDataFromPlugin);
  
  ChangeArtboard(0);
  
  csInterface.dispatchEvent(panelLoadedEvent);
  
  }
  );

function ReceiveDataFromPlugin(event)
{
    jsonArtboardData = event.data;
    //jsonArtboardData = JSON.stringify(event.data, null, '\t');
    //alert(json);
}

function ChangeArtboard(direction)
{
    var data = {
        "artboard-id"     :       currArtboardId
    };
    changeArtboardEvent.data = JSON.stringify(data);
    csInterface.dispatchEvent(changeArtboardEvent);
}

function SendDataToIllustrator()
{

}