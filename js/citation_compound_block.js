(function ($) {
    
   
    
    Drupal.behaviors.may_bragdon_citation_updater = {
        attach: function (context, settings) {
            // Monkey patch Drupal.settings.islandora_paged_tei_seadragon_update_page
            // to update compound block to ensure we always get the current one.
            var old_page_update = Drupal.settings.islandora_paged_tei_seadragon_update_page;
            Drupal.settings.islandora_paged_tei_seadragon_update_page = function (pid, page_number) {
                // Drop out here if we are the most current request.
                if (pid === Drupal.settings.islandora_paged_tei_seadragon.current_page) {
                    return;
                }
                
                 //run the original function
                old_page_update(pid, page_number);
                
                // call out to get citation information
                $.ajax({
                    url: Drupal.settings.basePath + "may_bragdon/mods/" + pid,
                    cache: false,
                    success: function(response) {
                        // Drop out here if we are not the most current request.
                        if (pid !== Drupal.settings.islandora_paged_tei_seadragon.current_page) {
                            return;
                        }
                        if(response.mods && response.mods.Page){
                            $('#citation_page_number').html(response.mods.Page[0]);
                        } else {
                             $('#citation_page_number').html("Not Found");  
                        }
                    },
                    error: function(response){
                        console.log(response);
                    }
                });
            };
        }
        
    };
    
    //call the page load function after the first page load
    $(function() {
        if( $(".openseadragon-container").length){
            $(".openseadragon-container").css("position", "absolute");
        }
        
        if($("#islandora_paged_tei_seadragon_pager").length) {
            Drupal.settings.islandora_paged_tei_seadragon_update_page(
                $("#islandora_paged_tei_seadragon_pager").val(),
                $("#islandora_paged_tei_seadragon_pager").children("option:selected").text()
            );
        }
    });
})(jQuery);
