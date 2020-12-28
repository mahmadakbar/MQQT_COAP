<?php 

//karena setiap inputan ajax berupa array post/get maka akan dicek indeks tiap array untuk tiap2 kasus fungsi ajax
if(isset($_POST)){
$server   = "localhost";
$usernames = "root";
$passwords = "";
$database = "test_wsn"; // nama database
 
$mysqli = mysqli_connect($server, $usernames, $passwords, $database);

//Check error, jikalau error tutup koneksi dengan mysql
if (mysqli_connect_errno()) {
	echo 'Koneksi gagal, ada masalah pada : '.mysqli_connect_error();
	exit();
	mysqli_close($mysqli);
}

$kelembapan =  mysqli_escape_string($mysqli,$_POST['kelembapan']);
$suhu_c =  mysqli_escape_string($mysqli,$_POST['suhu_c']);
            //melakukan query insert ke dalam database dengan variabel input yang diisikan oleh user
            $hasilquery = mysqli_query($mysqli,"INSERT INTO dht_test(kelembapan,suhu_c) VALUES ('$kelembapan','$suhu_c')");
            //jika sudah melakukan query insert maka akan mengembalikan nilai string ke file tampil.php yang nantinya akan ditampilkan  melalui browser, namun baris coding ini masih belum menampung nilai error jika query tidak berhasil dijalankan (tugas anda*)
}

?>