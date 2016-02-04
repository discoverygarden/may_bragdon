(function ($) {
    Drupal.behaviors.rochdora_compound_updater = {
        attach: function (context, settings) {
            // Monkey patch Drupal.settings.islandora_paged_tei_seadragon_update_page
            // to update compound block to ensure we always get the current one.

            // Hide empty sidebar at start.
            if (!$('.islandora-compound-prev-next').length) {
                $('.sidebars').hide();
            }

            var old_page_update = Drupal.settings.islandora_paged_tei_seadragon_update_page;
            Drupal.settings.islandora_paged_tei_seadragon_update_page = function (pid, page_number) {
                // Drop out here if we are the most current request.
                if (pid === Drupal.settings.islandora_paged_tei_seadragon.current_page) {
                    return;
                }
                
                //run the original function
                old_page_update(pid, page_number);

                // Update compound block.
                $.ajax({
                    url: Drupal.settings.basePath + "rochdora/compound_block/" + pid,
                    cache: false,
                    success: function(response) {
                        // Drop out here if we are not the most current request.
                        if (pid !== Drupal.settings.islandora_paged_tei_seadragon.current_page) {
                            return;
                        }
                        // Kill the block.
                        $('.islandora-compound-prev-next').remove();
                        $('.sidebars').hide();
                        // Revive the block.
                        if (response) {
                            $('.sidebars').show();
                            $('#block-islandora-compound-object-compound-navigation').append(response);
                        }
                    }
                });
            };
        }
    };
})(jQuery);
