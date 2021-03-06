<?php

/**
 * @file
 * Menu callbacks.
 */

/**
 * Menu callback for returning mods data.
 */
function may_bragdon_object_mods(AbstractObject $object) {
    module_load_include('inc', 'islandora', 'includes/utilities');

    //try and load the mods
    $mods = $object->getDatastream("MODS");

    // Need at least title, full name of at least one author, publication year.
    if (!isset($mods) || !islandora_datastream_access(ISLANDORA_VIEW_OBJECTS, $mods)) {
        return drupal_json_output(array('mods' => null));
    } else {
        $info = get_data($object);
        return drupal_json_output(array('mods' => $info));
    }
}

/**
 * Menu callback for returning mods data.
 */
function may_bragdon_feedback() {
    $email = $_POST["email"];
    $content = $_POST["content"];
    $location = $_POST["location"];
    $unused = $_POST["unused"];

    if ((!empty($content)) && empty($unused) && filter_var($email, FILTER_VALIDATE_EMAIL)) {
        $to = "mbragdon@library.rochester.edu";
        $email_subject = "May bragdon feedback";
        $email_body = $content . " \n\n sent from page: " . $location;
        $headers = "From: $email\n";
        $headers .= "Reply-To: $email";
        mail($to, $email_subject, $email_body, $headers);
    }


    return drupal_json_output(array('email' => $email, 'content' => content, 'location' => $location));
}

/**
 * Menu callback for returning mods data.
 */
function may_bragdon_ography($identifier) {
    $ographySrc = "";
    $result = null;
    $ographyLookUpID = null;
    $ographyPreFix = "";
    $ographyID = "";

    $ographyLookUpID = explode(":", $identifier);
    $ographyPreFix = $ographyLookUpID[0];       // echo($ographyPreFix . "<br>");
    $ographyID = $ographyLookUpID[1];           // echo($ographyID . "<br>");

    $xmlDoc = new DOMDocument;

    global $base_url;
    $site_path = $base_url . base_path();

    switch ($ographyPreFix) {
        case "pla":
            // echo "in the PLACES OGRPAPHY FILE<br>";
            $ographySrc = $site_path . "islandora/object/maybragdon%3Aroot/datastream/PLACES/view";
            $xpathPhrase = '//t:place[@xml:id="' . $ographyID . '"]';
            $result = retrieveOgraphyRecord($ographySrc, $xpathPhrase, $xmlDoc);
            //$record = retrieveOgraphyRecord($ographySrc, $xpathPhrase);
            //$result = "<h2>" . $record[0]->placeName[1] . "</h2><i>" . $record[0]->placeName[0]->address->addrLine . $record[0]->note;
            break;
        case "psn":
            // echo "in the PERSON OGRAPHY FILE<br>";
            $ographySrc = $site_path . "islandora/object/maybragdon%3Aroot/datastream/PERSONS/view";
            $xpathPhrase = '//t:person[@xml:id="' . $ographyID . '"]';
            $result = retrieveOgraphyRecord($ographySrc, $xpathPhrase, $xmlDoc);
            //$record = retrieveOgraphyRecord($ographySrc, $xpathPhrase);
            // $result = array('name' => $record[0]->persName[1], 'note' => $record[0]->note);
            //$result = "<h2>" . $record[0]->persName[1] . "</h2><i>" . $record[0]->note;
            break;
        case "evn":
            // echo "in the EVENTS OGRAPHY FILE<br>";
            $ographySrc = $site_path ."islandora/object/maybragdon%3Aroot/datastream/EVENTS/view";
            $xpathPhrase = '//t:event[@xml:id="' . $ographyID . '"]';
            $result = retrieveOgraphyRecord($ographySrc, $xpathPhrase, $xmlDoc);
            //$record = retrieveOgraphyRecord($ographySrc, $xpathPhrase);
            //$result = "<h2>" . $record[0]->label[1] . "</h2><i>" . $record[0]->note;
            break;
        case "nam":
            // echo "in the NAMES OGRAPHY FILE<br>";
            $ographySrc = $site_path . "islandora/object/maybragdon%3Aroot/datastream/NAMES/view";
            $xpathPhrase = '//t:name[@xml:id="' . $ographyID . '"]';
            $result = retrieveOgraphyRecord($ographySrc, $xpathPhrase, $xmlDoc);
            break;
    }
    // $data = $xmlDoc->saveXML($result->item(0));


    /*
    return drupal_json_output(array(
        '$site_path' => $site_path,
        '$ographyPreFix' => $ographyPreFix,
        '$ographySrc' => $ographySrc,
        '$xpathPhrase' => $xpathPhrase,
        '$data' => $data,
        '$ographyLookUpID' => $ographyLookUpID));*/
    // print $data;
    print $result;
}

function retrieveOgraphyRecord($ographySrc, $xpathPhrase, $xmlDoc) {

    //$xml = new SimpleXMLElement(file_get_contents());
    $xmlDoc->load($ographySrc);
    $xpath = new DOMXPath($xmlDoc);
    $xpath->registerNamespace('t', 'http://www.tei-c.org/ns/1.0');

    // error checking for when there are no matches
    $item = "default";
    $item = $xpath->query($xpathPhrase)->item(0);
    if ($item != null && $item != "default"){
      $record = $item->getAttribute('xml:id') ? $xmlDoc->saveXML($item) : $xmlDoc->saveXML('');
    } elseif ($item != "default") {
      $record = "Research is ongoing. Please check back soon.";
    }
    return $record;

}

/**
 * Get the file name stored in the tei
 * 
 * @param type $object
 * @return type
 */
function getFigureInfo($object){
    
    $figure_file_name = null;
    if(isset($object["MODS"])){
        $xml = simplexml_load_string($object["MODS"]->content);
        $xml->registerXPathNamespace('mods', 'http://www.loc.gov/mods/v3');
        
        $image_identifiers = $xml->xpath('(/mods:mods/mods:identifier[@type="fileName"])');
       
        $pattern = "/\.jpg$/";
        foreach($image_identifiers as $identifier ){
            $fileName = (string)$identifier;
            if(preg_match($pattern, $fileName)){
               $figure_file_name = $fileName;
            }
        }      
    }
    return $figure_file_name;
}

function getTeiFigures($object, $file_name) {
    $markup = null;
    if (isset($object["TEI"])) {
        $xml = simplexml_load_string($object["TEI"]->content);
        $xml->registerXPathNamespace('tei', 'http://www.tei-c.org/ns/1.0');
        $image_figures = $xml->xpath('(/tei:TEI/tei:teiHeader/tei:fileDesc/tei:sourceDesc/tei:msDesc/tei:physDesc/tei:additions/tei:figure[tei:graphic[@url="' . $file_name . '"]])');

        if (count($image_figures) > 0) {
            $node = $image_figures[0];
            $path = drupal_get_path('module', 'may_bragdon') . '/tei_transforms';
            $style = file_create_url("$path/tei2html.xslt");
            $islandora_url = variable_get('islandora_base_url', 'http://localhost:8080/fedora');
            $tomcat_url = str_replace('/fedora', '', $islandora_url);
            $tempfile_object = file_save_data($node->asXML(), file_default_scheme() . '://');
            $source = file_create_url($tempfile_object->uri);
            $saxon_url = url("$tomcat_url/saxon/SaxonServlet", array(
                'absolute' => TRUE,
                'external' => TRUE,
                'query' => array(
                    'style' => $style,
                    'source' => $source,
                    'clear-stylesheet-cache' => 'yes',
                    'css_base' => file_create_url($path),
                ),
            ));

            $result = drupal_http_request($saxon_url);
            file_delete($tempfile_object);
            if (isset($result->error)) {
                $markup = "$saxon_url $result->error";
            } else {

                // don't show errors
                libxml_use_internal_errors(true);
                
                $markup = $result->data;
            }
        }
    }
    return $markup;
}
