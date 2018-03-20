function refreshPreview() {
    if (document.querySelector('input[name="new_device_type"]:checked').value == 5 || document.querySelector('input[name="new_device_type"]:checked').value == 8) {
        document.getElementById('orientation').style.display = "none";
        document.getElementById('upload').style.display = "initial";
    } else {
        document.getElementById('orientation').style.display = "initial";
        document.getElementById('upload').style.display = "none";
    }
    document.getElementById('preview').src="../get_png.php?mac_address=" + document.getElementById('mac_address').value + "&layout=" + document.querySelector('input[name="new_device_type"]:checked').value
}

inputs = document.getElementsByTagName('input');
for(i=0; i<inputs.length; i++) {
    inputs[i].addEventListener('change', refreshPreview, false);
}
        

