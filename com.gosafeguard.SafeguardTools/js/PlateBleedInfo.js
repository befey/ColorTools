var csInterface = new CSInterface(); 

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
  csInterface.dispatchEvent(panelLoadedEvent);
  }
  );

function ReceiveDataFromPlugin(event)
{
    //TODO: CHANGE THIS FOR BLEED INFO PLUGIN
    //var json = JSON.parse(event.data);
    var json = JSON.stringify(event.data, null, '\t');
    alert(json);
}

function ChangeArtboard(direction)
{
    var data = {
        "artboard-id"     :       direction
    };
    changeArtboardEvent.data = JSON.stringify(data);
    csInterface.dispatchEvent(changeArtboardEvent);
}

function SendDataToIllustrator()
{

}