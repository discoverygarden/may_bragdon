# May Bragdon Project
## Handy SOLR Queries

### Compound Objects without MODS Datastreams
Query: `RELS_EXT_hasModel_uri_s:*compound* AND -fedora_datastreams_ms:*MODS*`

Translation:  Find objects with the compound model (Parent Inclusions) that do NOT have a MODS datastream.

### Find Pages with Updated TEI*
Query: `-fedora_datastream_latest_TEI_ID_ms:TEI.0 AND RELS_EXT_hasModel_uri_s:*pageCModel AND fedora_datastream_latest_TEI_ID_ms:[* TO *]`
Fields:  PID,fgs_label_s,mods_relatedItem_host_identifier_local_s,fedora_datastream_version_TEI_ID_ms,RELS_EXT_isMemberOf_uri_s

Translation:  Find objects with the page model (diary pages) that have a TEI datastream that has a version number other than 0 (0 is initial datastream version)

### Query Header
Query: `query string goes here`
Translation:  What this query does goes here
