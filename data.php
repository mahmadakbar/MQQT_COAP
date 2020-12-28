<?php

  $dbhost = "localhost";
  $dbname = "test_wsn";
  $dbuser = "root";
  $dbpass = "";

  /* We first connect to our database */
  $connection = mysqli_connect($dbhost,$dbuser,$dbpass,$dbname);

  /* Capture connection error if any */
  if (mysqli_connect_errno($connection)) {
          echo "Failed to connect to DataBase: " . mysqli_connect_error();
      }
  else {

    /* Declare an array containing our data points */
    $data_points = array();

    /* Usual SQL Queries */
    $query = "SELECT `time`,`kelembapan`,`suhu_c` FROM `dht_test`";

    $result = mysqli_query($connection, $query);

    while($row = mysqli_fetch_array($result)){        
      /* Push the results in our array */
      $point = array("datta0" =>  $row['time'] ,"datta1" =>  $row['kelembapan'] ,"datta2" =>  $row['suhu_c']);
      array_push($data_points, $point);
    }

    /* Encode this array in JSON form */
    echo json_encode($data_points, JSON_NUMERIC_CHECK);
  }
  mysqli_close($connection);
?>