<?php 

$opts = array(
  'http'=>array(
    'method'=>"GET",
    'header'=>"Accept-language: en\r\n" .
                "User-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1)\r\n",
  )
);
 
$context = stream_context_create($opts);

// Open the file using the HTTP headers set above
$file = file_get_contents('https://api.vk.com/api.php?oauth=1&method=groups.getById&group_id=139613247&version=5.74&fields=members_count');
//echo &#36;file;
 
$target  = '"members_count":';
$target1 = ',';

//echo strlen($file);
$pos = strstr($file, $target);
$pos = strstr($pos, $target1, true);
$pos = substr($pos, strlen($target));
echo '$'.$pos;