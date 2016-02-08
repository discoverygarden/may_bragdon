<?php
/**
 * @file
 * Menu callbacks.
 */

/**
 * Menu callback for returning a compound object block.
 */
function may_bragdon_object_mods(AbstractObject $object) {
  module_load_include('inc', 'islandora', 'includes/utilities');

  //try and load the mods
  $mods = $object->getDatastream("MODS");
  
  // Need at least title, full name of at least one author, publication year.
  if (!isset($mods) || !islandora_datastream_access(ISLANDORA_VIEW_OBJECTS, $mods)) {
      return drupal_json_output(array('mods' => null));
  }
  else {
    $info = get_data($object);  
    return drupal_json_output(array('mods' => $info));

  }
}
