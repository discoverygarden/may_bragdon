<?php

/**
 * Theme function for the compound navigation block parts.
 */
function may_bragdon_template_preprocess_islandora_compound_prev_next(array &$variables) { 
  
  module_load_include('inc', 'may_bragdon', 'includes/utilities');
  $themed_siblings = array();
  $image_path = drupal_get_path('module', 'islandora');
  $folder_image_path = "$image_path/images/folder.png";

  // If the parent hasModel islandora:compoundCModel,
  // the first child is typically identical to the parent;
  // do not show the parent TN.

  // If the parent does not have hasModel islandora:compoundCModel,
  // the first child and parent are different;
  // show the parent TN.

  $variables['parent_tn'] = NULL;
  $variables['original_tn'] = NULL;
  $parent_object = islandora_object_load($variables['parent_pid']);
  
  
  $variables['page_number'] = getRelsData($parent_object, 'isPageNumber');
  $variables['diary_id'] =  getRelsData($parent_object, 'isPageOf');
  
  if ($parent_object && !in_array(ISLANDORA_COMPOUND_OBJECT_CMODEL, $parent_object->models)) {
    if (isset($parent_object['JPG']) && islandora_datastream_access(ISLANDORA_VIEW_OBJECTS, $parent_object['JPG'])) {
      $variables['parent_tn'] = 'islandora/object/' . $parent_object->id . '/datastream/JPG/view';
    }
    else {
      $variables['parent_tn'] = $folder_image_path;
    }
  }
  
  $params = drupal_get_query_parameters();
  
  //see if an original view exists
  $original_pid = may_bragdon_lookup_occluded_pid($parent_object->id);
  
  $variables['has_occluded_thumb'] = false;
  
  //do the oposite of what is shown - if un-occluded view is shown
  //show occluded thumb - othwerwise show non-occluded thumb
  $variables['show_occluded_thumb'] = true;
  
  if($original_pid){
      
      $variables['has_occluded_thumb'] = true;
      
      if (isset($params['occluded']) && $params['occluded'] == 'true') {
          $variables['show_occluded_thumb'] = false;
      }
      
      $original_view = islandora_object_load($original_pid);
      if ($original_view && !in_array(ISLANDORA_COMPOUND_OBJECT_CMODEL, $parent_object->models)) {
        if (isset($original_view['JPG']) && islandora_datastream_access(ISLANDORA_VIEW_OBJECTS, $original_view['JPG'])) {
        $variables['original_tn'] = 'islandora/object/' . $original_view->id . '/datastream/JPG/view';
      }
      else {
        $variables['original_tn'] = $folder_image_path;
      }
    }
  }

  // If the parent TN is not set, only show children if there's more than one.
  // If the parent TN is set, show all children even if there's only one.
  
  if (($variables['child_count'] > 1 && !$variables['parent_tn']) || 
          ($variables['child_count'] > 0 && $variables['parent_tn'])) {
    
    // iterate through the first level siblings  
    for ($i = 0; $i < count($variables['siblings']); $i += 1) {
      $child_pid = $variables['siblings'][$i]; 
      $child = theme_rochdora_get_children($child_pid);
      if(!empty($child)){
           $themed_siblings[] = $child;
      }
     
    }
  }
  
  //see if it's direct children have images
  if( count($themed_siblings) == 0 ){
    $themed_siblings[] = theme_rochdora_get_children($variables['parent_pid']);
  }
  
  
  $variables['themed_siblings'] = $themed_siblings;
 
}

function theme_rochdora_get_children($pid){
    $image_path = drupal_get_path('module', 'islandora');
    $folder_image_path = "$image_path/images/folder.png";
    
    //hold the themed children information
    $themed_children = [];
    
    // load the child pids for the given pid
    $children_pids = islandora_compound_object_get_parts($pid);
    
    foreach($children_pids as $child_pid){
        
        //load the sibling object
        $sibling_object = islandora_object_load($child_pid);
        
        $child_info = array();
        $child_info['class'][] = 'active';
        $child_info['pid'] = $child_pid;
        $child_info['parent_pid'] = $pid;

        if(isset($sibling_object['JPG']) && 
           islandora_datastream_access(ISLANDORA_VIEW_OBJECTS, $sibling_object['JPG'])){
               $child_info['JPG'] = 'islandora/object/' . $child_pid . '/datastream/JPG/view';
        } else {
            // Object either does not have a thumbnail or it's restricted show a
            // default in its place.
            $child_info['JPG'] = $folder_image_path;
        }
       
        $child_info['label'] = $sibling_object->label;
           
        $themed_children[] = $child_info;
    }     
    return $themed_children;
    
}


