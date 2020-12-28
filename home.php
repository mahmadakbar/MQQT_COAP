<?php
  	include "singledata.php";
 ?>
<!DOCTYPE html>
<html>
	<head>
		<title>Graph 1 Coba</title>
	</head>
<body>

	<div id="chartContainer"></div>
	<div id="timeNdate">
		<div id='date-part' style="margin-right: 0.5em"></div>
    	<div id='time-part'></div>	
	</div>
	<div id="monitor">
	
	</div>


	<!-- import assets -->
	<link rel="stylesheet" href="assets/styles.css">
	<script type="text/javascript" src="assets/moment.js"></script>
	<script type="text/javascript" src="assets/jquery.min.js"></script>
	<script type="text/javascript" src="assets/canvasjs.min.js"></script>
	<script type="text/javascript" src="assets/linegraph.js"></script>

	<script>
		$(document).ready(function(){
			var interval = setInterval(function() {
				var momentNow = moment();
				$('#date-part').html(momentNow.format('DD MMMM YYYY'));
				$('#time-part').html(momentNow.format('hh:mm:ss'));
			}, 100);
		  	function refreshMonitor(){
			    $('#monitor').load('monitor.php', function(){

			       	setTimeout(refreshMonitor, 1000);
			    });
			}

			refreshMonitor();
		});
	</script>


</body>

</html>