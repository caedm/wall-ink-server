//this particular function was taken off of stackoverflow.
function sortTable(column) {
  var table, rows, switching, i, x, y, shouldSwitch;
  table = document.getElementById("devices");
  switching = true;
  /* Make a loop that will continue until
  no switching has been done: */
  while (switching) {
    // Start by saying: no switching is done:
    switching = false;
    rows = table.getElementsByClassName("device");
    /* Loop through all table rows (except the
    first, which contains table headers): */
    for (i = 0; i < (rows.length - 1); i++) {
      // Start by saying there should be no switching:
      shouldSwitch = false;
      /* Get the two elements you want to compare,
      one from current row and one from the next: */
      x = rows[i].getElementsByTagName("TD")[column];
      y = rows[i + 1].getElementsByTagName("TD")[column];
      // Check if the two rows should switch place:
      if (x.innerHTML.toLowerCase() > y.innerHTML.toLowerCase()) {
        // If so, mark as a switch and break the loop:
        shouldSwitch = true;
        break;
      }
    }
    if (shouldSwitch) {
      /* If a switch has been marked, make the switch
      and mark that a switch has been done: */
      rows[i].parentNode.insertBefore(rows[i + 1], rows[i]);
      switching = true;
    }
  }
};

function refreshFilters() {
    var production = document.getElementsByClassName('production');
    var notProduction = document.getElementsByClassName('notproduction');
    var showProduction = document.getElementById('showProduction').checked;
    var showNotProduction = document.getElementById('showNotProduction').checked;
    for (i = 0; i < production.length; i++) {
        if (showProduction) {
            production[i].style.display = "table-row";
        } else {
            production[i].style.display = "none";
        }
    }
    for (i = 0; i < notProduction.length; i++) {
        if (showNotProduction) {
            notProduction[i].style.display = "table-row";
        } else {
            notProduction[i].style.display = "none";
        }
    }
}

var showProduction = document.getElementById("showProduction");
var showNotProduction = document.getElementById("showNotProduction");
showProduction.addEventListener('change', function() {
    refreshFilters();
});
showNotProduction.addEventListener('change', function() {
    refreshFilters();
});

//make sure these start checked
document.getElementById('showProduction').checked = true;
document.getElementById('showNotProduction').checked = true;

refreshFilters();
