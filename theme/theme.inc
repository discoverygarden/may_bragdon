<?php

/**
 * Theme function for the compound navigation block parts.
 */
function template_preprocess_islandora_compound_prev_next(array &$variables) { 
  
 drupal_add_js(drupal_get_path('module', 'islandora_compound_object') . '/js/caption_width.js');

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
  $parent_object = islandora_object_load($variables['parent_pid']);
  if ($parent_object && !in_array(ISLANDORA_COMPOUND_OBJECT_CMODEL, $parent_object->models)) {
    if (isset($parent_object['TN']) && islandora_datastream_access(ISLANDORA_VIEW_OBJECTS, $parent_object['TN'])) {
      $variables['parent_tn'] = 'islandora/object/' . $parent_object->id . '/datastream/TN/view';
    }
    else {
      $variables['parent_tn'] = $folder_image_path;
    }
  }

  // If the parent TN is not set, only show children if there's more than one.
  // If the parent TN is set, show all children even if there's only one.

  if (($variables['child_count'] > 1 && !$variables['parent_tn']) || 
          ($variables['child_count'] > 0 && $variables['parent_tn'])) {
      
    // iterate through the first level siblings  
    for ($i = 0; $i < count($variables['siblings']); $i += 1) {
      // see if this has other children
      $themed_siblings = array_merge($themed_siblings, 
              theme_rochdora_get_children($variables['siblings'][$i]));
    }
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
        
        //load the child object
        $child_object = islandora_object_load($child_pid);
        
        $sibling_object = islandora_object_load($child_pid);
        
        $child_info = array();
        $child_info['class'][] = 'active';
        $child_info['pid'] = $child_pid;

        if(isset($sibling_object['TN']) && 
           islandora_datastream_access(ISLANDORA_VIEW_OBJECTS, $sibling_object['TN'])){
               $child_info['TN'] = 'islandora/object/' . $child_pid . '/datastream/TN/view';
        } else {
            // Object either does not have a thumbnail or it's restricted show a
            // default in its place.
            $child_info['TN'] = $folder_image_path;
        }
       
        $child_info['label'] = $sibling_object->label;
           
        $themed_children[] = $child_info;
    }     
    return $themed_children;
    
}


/**
 * Implements theme_element().
 */
function theme_islandora_compound_object_draggable_table(array $variables) {
  $form = $variables['form'];
  $rows = array();
  $weight_class = 'islandora-page-weight';
  $table = &$form['table'];
  foreach (element_children($table['rows']) as $id) {
    $row = &$table['rows'][$id];
    $row['weight']['#attributes'] = array('class' => array($weight_class));
    $rows[] = array(
      'data' => array(
        drupal_render($row['label']),
        drupal_render($row['pid']),
        drupal_render($row['weight']),
      ),
      'class' => array('draggable'),
    );
  }
  $table_id = 'islandora-reorder-objects-table';
  $output = theme('table', array(
              'header' => $table['#header'],
              'rows' => $rows,
              'attributes' => array('id' => $table_id)));
  $output .= drupal_render_children($form);
  drupal_add_tabledrag($table_id, 'order', 'sibling', $weight_class);
  return $output;  
}


