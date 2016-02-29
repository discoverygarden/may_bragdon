<?php
/**
 * @file
 * islandora-compound-object-prev-next.tpl.php
 *
 * @TODO: needs documentation about file and variables
 * $parent_label - Title of compound object
 * $child_count - Count of objects in compound object
 * $parent_url - URL to manage compound object
 * $previous_pid - PID of previous object in sequence or blank if on first
 * $next_pid - PID of next object in sequence or blank if on last
 * $siblings - array of PIDs of sibling objects in compound
 * $themed_siblings - array of siblings of model
 *    array(
 *      'pid' => PID of sibling,
 *      'label' => label of sibling,
 *      'TN' => URL of thumbnail or default folder if no datastream,
 *      'class' => array of classes for this sibling,
 *    )
 */
?>
<div class="islandora-compound-prev-next">
   
    <div id="roch-tei-viewer-occluded" class="manuscript-view">
            <?php if($show_occluded_thumb):?>
                <?php if ($original_tn): ?>
                    <?php
                        print l( 
                                theme_image(
                                        array(
                                            'path' => $original_tn,
                                            'attributes' => array(),
                                        )
                                ), 'islandora/object/' . $diary_id , array('html' => TRUE, 
                                    'query' => array('islandora_paged_content_page' => $page_number, 'occluded' => 'true'))
                        );
                ?>
                <?php endif; ?>
            <?php else:?>
                <?php if($has_occluded_thumb && $parent_tn)
                    print l( 
                         theme_image(
                            array(
                                 'path' => $parent_tn,
                                 'attributes' => array(),
                            )
                        ), 'islandora/object/' . $diary_id, array('html' => TRUE, 
                            'query' => array('islandora_paged_content_page' => $page_number))
                    );
                ?>
            <?php endif;?>
    </div>
   
    <?php if (user_is_logged_in()): ?>
        <span class="islandora-compound-title"><?php print t('Part of: @parent (@count @objects)', 
                array('@parent' => $parent_label, '@count' => $child_count, 
                    '@objects' => format_plural($child_count, 'object', 'objects'))); ?>            
            <?php if ($parent_url): ?>
                <?php print l(t('manage parent'), $parent_url); ?>
            <?php endif; ?>
                       
        </span><br/>

    <?php endif; ?>
    <?php if (count($themed_siblings) > 0): ?>
        <div class="islandora-compound-thumbs">
            <?php foreach ($themed_siblings as $sibling): ?>
                <div class="bragdon-child-grouping">
                <?php foreach($sibling as $key=>$child): ?>
                    <div class="inclusion-label label-<?php print $key?>"><?php print $child['label']; ?></div>
                    <div class="islandora-compound-thumb <?php print(str_replace(' ', '_', trim(strtolower($child['label'])))); ?>">
                        <?php
                        print l(
                                    theme_image(
                                            array(
                                                'path' => $child['TN'],
                                                'attributes' => array('class' => $child['class']),
                                            )
                                    ), 'islandora/object/' . $child['pid'], array('html' => TRUE)
                        );
                        ?>
                    </div>
                 <?php endforeach; // each themed_siblings ?>
                </div>
            <?php endforeach; // each themed_siblings ?>
        </div> <!-- // islandora-compound-thumbs -->
    <?php endif; // count($themed_siblings) > 0  ?>   
 
</div>