var csInterface = new CSInterface();
var jsonArtboardData;
var storedArtboardPrint = [];

// Create events for the button presses
var makePdfEvent = new CSEvent("com.gosafeguard.SafeguardTools.PrintToPdf.makepdfbutton", "APPLICATION", "ILST", "PrintToPdf");

var cancelEvent = new CSEvent("com.gosafeguard.SafeguardTools.PrintToPdf.cancelbutton", "APPLICATION", "ILST", "PrintToPdf");

var resultsBackEvent = new CSEvent("com.gosafeguard.SafeguardTools.PrintToPdf.resultsback", "APPLICATION", "ILST", "PrintToPdf");

var panelLoadedEvent = new CSEvent("com.gosafeguard.SafeguardTools.PrintToPdf.panelloaded", "APPLICATION", "ILST", "PrintToPdf");

var noArtboardsSelectedEvent = new CSEvent("com.gosafeguard.SafeguardTools.PrintToPdf.noartboardsselected", "APPLICATION", "ILST", "PrintToPdf");

$(function()
  {
  csInterface.setWindowTitle("Print To PDF");
  $('#preset-select').change(function()
                             {
                                if ( $('#preset-select').val() == 0 ) //Manufacturing
                                {
                                    $('#separatefiles-check').prop('checked', true);
                                    $('.artboard-colors').each(function(i,e) {
                                                                ResetArtboardPrintToStored($(e));
                                                               });
                                }
                                else
                                {
                                    $('#separatefiles-check').prop('checked', false);
                                    $('.artboard-colors').each(function(i,e) {
                                                                TurnOnAllArtboardPrint($(e));
                                                               });
                                }
                             });

  $('#allpages-check').change(function()
                              {
                                if ($(this).is(':checked'))
                                {
                                    $("#range-text").css("color","gray");
                                }
                                else
                                {
                                    $("#range-text").css("color","black");
                                    $("#range-text").focus();
                                }
                              });
  
  $('#range-text').on('focus', function()
                      {
                      $("#allpages-check").attr('checked', false);
                      $("#range-text").css("color","black");
                      
                      $("#range-text")
                      .one('mouseup.mouseupSelect', function() {
                           $("#range-text").select();
                           return false;
                           })
                      .one('mousedown', function() {
                           // compensate for untriggered 'mouseup' caused by focus via tab
                           $("#range-text").off('mouseup.mouseupSelect');
                           })
                      .select();
                      });
  
  $("#colorlist-textarea").on( "click", ".artboard-colors", function()
                              {
                              ToggleArtboardPrint($(this));
                              });
  
  
  csInterface.addEventListener("com.gosafeguard.SafeguardTools.PrintToPdf.resultsback", onResultsBack);
  csInterface.addEventListener("com.gosafeguard.SafeguardTools.PrintToPdf.clearresultbox", clearResultBox);
  csInterface.addEventListener("com.gosafeguard.SafeguardTools.PrintToPdf.forcepanelclose",
                               function(event)
                               {
                               csInterface.closeExtension();
                               });
  csInterface.addEventListener("com.gosafeguard.SafeguardTools.PrintToPdf.datafromplugin", ReceiveDataFromPlugin);
  
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

function VerifySettings()
{
    var flag = false;
    for (var j = 0; j < jsonArtboardData.dto.plates.length; j++)
    {
        if (jsonArtboardData.dto.plates[j].shouldPrint)
        {
            flag = true;
            sendDataToIllustrator();
            break;
        }
    }
    if (flag == false)
    {
        csInterface.dispatchEvent(noArtboardsSelectedEvent);
    }
}

function sendDataToIllustrator()
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

function clearResultBox(event)
{
    $("#results-textarea").text('');
}

function PutColorList(data)
{
    $("#colorlist-textarea").html(function()
                                  {
                                      var newHtml = "";

                                      for (var j = 0; j < data.dto.plates.length; j++)
                                      {
                                          newHtml += "<div class='artboard-colors";
                                          if (jsonArtboardData.dto.plates[j].shouldPrint == false ||
                                              data.dto.plates[j].c.length == 0)
                                          {
                                            newHtml += " artboard-deselected";
                                          }
                                  
                                          newHtml += "' id='ab-" + j + "'>";
                                  
                                          if (data.dto.plates[j].c.length == 0)
                                          {
                                            newHtml += "<div class='colorName'>";
                                            newHtml += "NO IMPRINT";
                                            newHtml += "</div>";
                                          }
                                  
                                          for (var i = 0; i < data.dto.plates[j].c.length; i++)
                                          {
                                              color = data.dto.plates[j].c[i];
                                              newHtml += "<div class='colorName'>";
                                              if (color.colorName == "CMYK")
                                              {
                                                newHtml += "<div class='cmyk'>";
                                                newHtml += color.colorName;
                                                newHtml += "</div>";
                                              }
                                              else
                                              {
                                                newHtml += color.colorName;
                                              }
                                              newHtml += "</div>";
                                          }
                                          newHtml += "<div class='artboard-id'>" + data.dto.plates[j].artboardName + "</div></div>";
                                      }
                                      return newHtml;
                                  });
}

function ToggleArtboardPrint(artboardDiv)
{
    var index = parseInt(artboardDiv.attr('id').slice(3), 10);
    jsonArtboardData.dto.plates[index].shouldPrint = !jsonArtboardData.dto.plates[index].shouldPrint;

    artboardDiv.toggleClass("artboard-deselected");
}

function TurnOnAllArtboardPrint(artboardDiv)
{
    var index = parseInt(artboardDiv.attr('id').slice(3), 10);
    storedArtboardPrint[index] = jsonArtboardData.dto.plates[index].shouldPrint;
    jsonArtboardData.dto.plates[index].shouldPrint = true;

    artboardDiv.toggleClass("artboard-deselected", false);
}

function ResetArtboardPrintToStored(artboardDiv)
{
    var index = parseInt(artboardDiv.attr('id').slice(3), 10);
    jsonArtboardData.dto.plates[index].shouldPrint = storedArtboardPrint[index];
    
    if (storedArtboardPrint[index] && artboardDiv.hasClass("artboard-deselected"))
    {
        artboardDiv.toggleClass("artboard-deselected", false);
    }
    else if (!storedArtboardPrint[index] && artboardDiv.hasClass("artboard-colors"))
    {
        artboardDiv.toggleClass("artboard-deselected", true);
    }
}
