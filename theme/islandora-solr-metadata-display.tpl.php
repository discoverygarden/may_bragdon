<?php
/**
 * @file
 * Islandora_solr_metadata display template.
 *
 * Variables available:
 * - $solr_fields: Array of results returned from Solr for the current object
 *   based upon defined display configuration(s). The array structure is:
 *   - display_label: The defined display label corresponding to the Solr field
 *     as defined in the configuration in translatable string form.
 *   - value: An array containing all the result(s) found for the specific field
 *     in Solr for the current object when queried against Solr.
 * - $found: Boolean indicating if a Solr doc was found for the current object.
 * - $not_found_message: A string to print if there was no document found in
 *   Solr.
 *
 * @see template_preprocess_islandora_solr_metadata_display()
 * @see template_process_islandora_solr_metadata_display()
 */
?>
<?php if ($found):
  if (!(empty($solr_fields) && variable_get('islandora_solr_metadata_omit_empty_values', FALSE))):?>
  <div class="item-metadata">
      <?php foreach($solr_fields as $value): ?>
          <div class="item-metadata-element">
              <div class="item-metadata-label"><?php print $value['display_label']; ?></div>
              <div class="item-metadata-value"><?php print check_markup(implode("\n", $value['value']), 'islandora_solr_metadata_filtered_html'); ?></div>
          </div>
      <?php endforeach; ?>
  </div>

<?php endif; ?>
<?php else: ?>
    <?php //XXX: Hack in markup for message. ?>
    <div class="messages--warning messages warning">
      <?php print $not_found_message; ?>
    </div>
<?php endif; ?>