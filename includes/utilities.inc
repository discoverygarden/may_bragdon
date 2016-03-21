<?php

/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 * Helper to actually find an occluded version of an object.
 *
 * @param AbstractObject $object
 *   The object for which to check for an occluded version.
 */
function may_bragdon_lookup_occluded_pid($objectId) {
  $qp = new IslandoraSolrQueryProcessor();
  $qp->buildQuery(format_string('!field:"!value"', array(
    '!field' => 'RELS_EXT_isPartOf_uri_ms',
    '!value' => "info:fedora/{$objectId}",
  )));
  $qp->solrParams['fq'][] = format_string('!field:"!value"', array(
    '!field' => 'RELS_EXT_hasModel_uri_ms',
    '!value' => 'info:fedora/islandora:sp_large_image_cmodel',
  ));
  $qp->solrParams['fl'] = 'PID';
  $qp->solrStart = 0;
  $qp->solrLimit = 1;
  
  $qp->executeQuery();

  $results = $qp->islandoraSolrResult['response']['objects'];
 
  if (empty($results)) {
    return FALSE;
  }
  else {
    $result = reset($results);
    return $result['PID'];
  }
}

/**
 * Returns the string value of the relationship data
 *
 * @param object $object - FedoraObject
 * @param value $value - relationship data to find
 *   
 *
 * @return string|bool
 *   Returns the value if found otherwise if there is no such membership, returns FALSE.
 */
function getRelsData($object, $value) {
    if (!empty($object)) {
        $rels = $object->relationships->get(ISLANDORA_RELS_EXT_URI, $value);
        $rel = reset($rels);
        if ($rel) {
            return $rel['object']['value'];
        }
        return FALSE;
    }else{
        return FALSE;
    }
}