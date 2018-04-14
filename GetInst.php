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
$file = file_get_contents('https://www.instagram.com/carlsjr_belarus/?hl=tr', false, $context);

$before1  = 'og:description';
$before2 = 'content="';
$after = ' ';

$file = strstr($file, $before1);
$file = strstr($file, $before2);
$file = strstr($file, $after, true);
$file = substr($file, strlen($before2));
echo '$'.str_replace(',', '', $file);
?>