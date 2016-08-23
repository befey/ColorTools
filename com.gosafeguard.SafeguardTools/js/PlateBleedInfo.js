var csInterface = new CSInterface(); 
var jsonArtboardData;
var currArtboardId = 0;

// Create events for the button presses
var panelLoadedEvent = new CSEvent("com.gosafeguard.SafeguardTools.PlateBleedInfo.panelloaded", "APPLICATION", "ILST", "PlateBleedInfo");
var changeArtboardEvent = new CSEvent("com.gosafeguard.SafeguardTools.PlateBleedInfo.changeartboard", "APPLICATION", "ILST", "PlateBleedInfo");
var cancelEvent = new CSEvent("com.gosafeguard.SafeguardTools.PlateBleedInfo.cancelbutton", "APPLICATION", "ILST", "PlateBleedInfo");
var sendDataBackEvent = new CSEvent("com.gosafeguard.SafeguardTools.PlateBleedInfo.datafromext", "APPLICATION", "ILST", "PlateBleedInfo");



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
    jsonArtboardData = event.data;
    
    LoadJsonDataForCurrentArtboard();
    ChangeArtboard(currArtboardId);
}

function ChangeArtboard(direction)
{
    StoreCurrentArtboardData();
    
    currArtboardId = (currArtboardId + direction) % jsonArtboardData.plateBleedInfoDTO.length;
    LoadJsonDataForCurrentArtboard();
    var data = {
        "artboard-id"     :       currArtboardId
    };
    changeArtboardEvent.data = JSON.stringify(data);
    csInterface.dispatchEvent(changeArtboardEvent);
}

function SendDataToIllustrator()
{
    StoreCurrentArtboardData();
    
    sendDataBackEvent.data = jsonArtboardData;
    csInterface.dispatchEvent(sendDataBackEvent);
}

function LoadJsonDataForCurrentArtboard()
{
    $("#artboard-name").val(jsonArtboardData.dto.plates[currArtboardId].artboardName);
    $("#artboard-number").text(currArtboardId + 1);
    $("#tickmark-select").val(jsonArtboardData.dto.plates[currArtboardId].tmStyle);
    
    $("#inks").html(function() {
                    var newHtml = "";
                    for (var i = 0; i < jsonArtboardData.dto.plates[currArtboardId].c.length; i++)
                    {
                        color = jsonArtboardData.dto.plates[currArtboardId].c[i];
                        newHtml += "<div class='trow'><div id='colorname-text" + i + "' class='tcell1'>" +
                        color.colorName +
                        "</div>" +
                        "<div class='tcell2'>" +
                        "<select id='inktype-select" + i + "'>" +
                        "<option value='0'></option>" +
                        "<option value='1'>Flat</option>" +
                        "<option value='2'>Thermo</option>" +
                        "<option value='3'>Foil</option>" +
                        "<option value='4'>Emboss</option>" +
                        "</select>" +
                        "</div></div>";
                    }
                    return newHtml;
                    });
    
    for (var i = 0; i < jsonArtboardData.dto.plates[currArtboardId].c.length; i++)
    {
        color = jsonArtboardData.dto.plates[currArtboardId].c[i];
        $("#inktype-select" + i + " option").eq(color.method).attr("selected", "selected");
    }
}

function StoreCurrentArtboardData()
{
    jsonArtboardData.dto.plates[currArtboardId].artboardName = $("#artboard-name").val();
    jsonArtboardData.dto.plates[currArtboardId].tmStyle = $("#tickmark-select").val();
    
    for (var i = 0; i < jsonArtboardData.dto.plates[currArtboardId].c.length; i++)
    {
        color = jsonArtboardData.dto.plates[currArtboardId].c[i];
        color.method = $("#inktype-select" + i).val();
    }
}