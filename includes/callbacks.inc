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
    $item = $xpath->query($xpathPhrase)->item(0);
    if ($item != null){
      $record = $item->getAttribute('xml:id') ? $xmlDoc->saveXML($item) : $xmlDoc->saveXML('');
    } else {
      $record = "Sorry, no information found.";
      $currentURL = "http://$_SERVER[HTTP_HOST]$_SERVER[REQUEST_URI]";
      $errorMessage = "May B Error on:  " + $currentURL + "\rID:  " + $item;
      mail('atraub@library.rochester.edu', 'May B Test Error Message', $message);
    }
    return $record;

}
