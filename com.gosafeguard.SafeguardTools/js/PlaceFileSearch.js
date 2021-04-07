var csInterface = new CSInterface();
var returnedResults = [];

// Create events for the button presses
var searchEvent = new CSEvent("com.gosafeguard.SafeguardTools.PlaceFileSearch.searchbutton", "APPLICATION", "ILST", "PlaceFileSearch");

var placeEvent = new CSEvent("com.gosafeguard.SafeguardTools.PlaceFileSearch.placefile", "APPLICATION", "ILST", "PlaceFileSearch");

var cancelEvent = new CSEvent("com.gosafeguard.SafeguardTools.PlaceFileSearch.cancelbutton", "APPLICATION", "ILST", "PlaceFileSearch");

var resultsBackEvent = new CSEvent("com.gosafeguard.SafeguardTools.PlaceFileSearch.resultsback", "APPLICATION", "ILST", "PlaceFileSearch");

var panelLoadedEvent = new CSEvent("com.gosafeguard.SafeguardTools.PlaceFileSearch.panelloaded", "APPLICATION", "ILST", "PlaceFileSearch");

$(function()
  {
  csInterface.setWindowTitle("Place File Search");
  
  csInterface.addEventListener("com.gosafeguard.SafeguardTools.PlaceFileSearch.resultsback", onResultsBack);
  csInterface.addEventListener("com.gosafeguard.SafeguardTools.PlaceFileSearch.forcepanelclose",
                               function(event)
                               {
                               csInterface.closeExtension();
                               });
  
  $(document).keypress(function(e) {
                       if(e.which == 13) {
                       event.preventDefault();
                       Search();
                       return false;
                       }
                       });
  
  $("#results-textarea").on( "click", ".path", function()
                              {
                              SendPathToPlugin($(this));
                              });
  
  csInterface.dispatchEvent(panelLoadedEvent);
  }
  );

function SendSearchStringToPlugin()
{
    searchEvent.data = $("#search-string").val();
    csInterface.dispatchEvent(searchEvent);
}

function onResultsBack(event)
{
    clearResultBox(event);
    
    var xmlData = $.parseXML(event.data);
    var $xml = $(xmlData);
    
    if ( $xml.find("path").length == 0 )
    {
        $("#results-textarea").append("No results found.");
    }
    
    $xml.find("path").each( function(index)
                             {
                             $("#results-textarea").append("<div class='path deselected'>" + this.textContent + "</div><br />");
                             });
    
    $("#search-string").focus().select();
}

function clearResultBox(event)
{
    $("#results-textarea").text('');
}

function Search()
{
    SendSearchStringToPlugin();
}

function SendPathToPlugin(pathDiv)
{
    placeEvent.data = pathDiv.text();
    csInterface.dispatchEvent(placeEvent);
}
