function refreshFilters() {
    var logs = document.getElementsByClassName('log');
    var filterForErrors = document.getElementById('filterForErrors').checked;
    for (i = 0; i < logs.length; i++) {
        var hidden = false;
        if (logs[i].textContent.includes("Error Code: 0") && filterForErrors) {
            hidden = true;
        }
        if (!logs[i].textContent.includes(document.getElementById("search").value)) {
            hidden = true;
        }
        if (hidden) {
            logs[i].style.display = "none";
        } else {
            logs[i].style.display = "initial";
        }
    }
}

var filterForErrorsCheckbox = document.getElementById("filterForErrors");
var search = document.getElementById("search");
filterForErrorsCheckbox.addEventListener('change', function() {
    refreshFilters();
});
search.addEventListener('change', function() {
    refreshFilters();
});

refreshFilters();
