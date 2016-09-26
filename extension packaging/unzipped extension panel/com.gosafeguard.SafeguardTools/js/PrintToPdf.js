var csInterface = new CSInterface();

// Create events for the button presses
var makePdfEvent = new CSEvent("com.gosafeguard.SafeguardTools.PrintToPdf.makepdfbutton", "APPLICATION", "ILST", "PrintToPdf");

var cancelEvent = new CSEvent("com.gosafeguard.SafeguardTools.PrintToPdf.cancelbutton", "APPLICATION", "ILST", "PrintToPdf");

var resultsBackEvent = new CSEvent("com.gosafeguard.SafeguardTools.PrintToPdf.resultsback", "APPLICATION", "ILST", "PrintToPdf");

var panelLoadedEvent = new CSEvent("com.gosafeguard.SafeguardTools.PrintToPdf.panelloaded", "APPLICATION", "ILST", "PrintToPdf");

$(function()
  {
  csInterface.setWindowTitle("Print To PDF");
  $('#preset-select').change(function()
                             {
                             if ( $('#preset-select').val() == 0 ) //Manufacturing
                             {
                             $('#separatefiles-check').prop('checked', true);
                             }
                             else
                             {
                             $('#separatefiles-check').prop('checked', false);
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
  
  $("#range-text").on('focus', function()
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
    PutColorList(event.data);
}

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

function PutColorList(data)
{
    $("#colorlist-textarea").html(function() {
                                  var newHtml = "";
                                  for (var j = 0; j < data.dto.plates.length; j++)
                                  {
                                    newHtml += "<div class='artboard-colors'>";
                                    for (var i = 0; i < data.dto.plates[j].c.length; i++)
                                    {
                                        color = data.dto.plates[j].c[i];
                                        newHtml += "<div class='colorName'>" + color.colorName + "</div>";
                                    }
                                    newHtml += "<div class='artboard-id'>" + data.dto.plates[j].artboardName + "</div></div>";
                                  }
                                  return newHtml;
                                  });
    
}
