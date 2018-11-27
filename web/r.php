<?php
if (preg_match('/^[[:alphanum:]]{1,100}$/', $_GET["r"]) === 1) {
    echo "<html>";
    echo "<meta http-equiv='refresh' content='1;url=https://reserve.et.byu.edu/reservations/Web/reservation.php?rid=";
    echo "$_GET[r]'>";
    echo "</html>";
} else {
    echo "regex doesn't match";
}
?>
