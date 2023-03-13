<?php

$conn = pg_connect("host=localhost dbname=coba1 user=postgres password=1119");
if ($conn) {
  echo 'Connection attempt success.';
} else {
  echo 'Connection attempt failed.';
}
$suhu = $_GET['data1'];
$kelembaban = $_GET['data2'];
$gasanalog = $_GET['data3'];
$gasdigital = $_GET['data4'];
$fuzzywa = $_GET['data5'];
$fuzzymsg = $_GET['data6'];


$sql = "INSERT INTO suhu(suhu)
VALUES ('$suhu');";

if (pg_query($conn, $sql)) {
  echo "New record created successfully";
} 
else {
  echo "Error: " . $sql;
}

$sql = "INSERT INTO kelembaban(kelembaban)
VALUES ('$kelembaban');";

if (pg_query($conn, $sql)) {
  echo "New record created successfully";
} 
else {
  echo "Error: " . $sql;
}

$sql = "INSERT INTO gas(gasanalog, gasdigital)
VALUES ('$gasanalog','$gasdigital');";

if (pg_query($conn, $sql)) {
    echo "New record created successfully";
} 
else {
    echo "Error: " . $sql;
}

$sql = "INSERT INTO fuzzy(wa, msg)
VALUES ('$fuzzywa','$fuzzymsg');";

if (pg_query($conn, $sql)) {
    echo "New record created successfully";
} 
else {
    echo "Error: " . $sql;
}


function test_input($data) {
$data = trim($data);
$data = stripslashes($data);
$data = htmlspecialchars($data);
return $data;
}
