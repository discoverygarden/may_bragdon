<?php

/**
 * @file
 * Render a bunch of objects in a list or grid view.
 */
?>
<div class="islandora-objects-grid clearfix">
 <?php foreach($objects  as $object): ?>
   <div class="diary-page-image-wrapper">
       <div class="islandora-object-thumb"><?php print $object['thumb']; ?></div>
       <div class="diary-page-number-wrapper">
           <div class="page-number-prefix">p.</div>
           <div class="diary-page-number"><?php print $object['page']; ?></div> 
       </div>
   </div>
 <?php endforeach; ?>
 <div class="clearfix"></div>   
    
 
</div>
