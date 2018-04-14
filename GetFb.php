<?php 
$opts = array(
  'http'=>array(
    'method'=>"GET",
    'header'=>"Accept-language: en\r\n" .
                "User-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1)\r\n",
              "Cookie: foo=bar\r\n"
  )
);
 
$context = stream_context_create($opts);

// Open the file using the HTTP headers set above
$file = file_get_contents('https://www.facebook.com/plugins/fan.php?connections=100&id=carlsjr.belarus', false, $context);
//echo &#36;file;
 
$target  = '_1drq';
$target1 = ';">';
$target2 = ' ';

$file = substr($file, 100000);
$temp = strstr($file, $target);
$temp = strstr($temp, $target1);
$temp = strstr($temp, $target2, true);
echo '$'.substr($temp, strlen($target1));
?>