<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="2.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

  <xsl:output method="html" indent="no" encoding="UTF-8"/>

  
  <xsl:template match="/">
    <div class="abstract">
        
          <xsl:apply-templates
            select="figure/head"
            mode="inline"/>
        
    </div>
  </xsl:template>

  <xsl:template match="*" mode="#all">
    <span>
      <xsl:call-template name="element_attributes"/>
      <xsl:apply-templates mode="#current"/>
    </span>
  </xsl:template>



  <xsl:template match="pb" mode="#all">
    <br class="pb"/>
  </xsl:template>


  <xsl:template match="caption" mode="#all">
    <div class="caption">
      <xsl:apply-templates/>
    </div>
  </xsl:template>

  <xsl:template match="unclear | illegible" mode="#all">
    <xsl:apply-templates/>
    <div class="note">
      Editorial Note <xsl:value-of select="local-name()"/>: <xsl:value-of select="current()"/>
    </div>
  </xsl:template>

  <xsl:template match="caption/text()" mode="#all">
    <xsl:value-of select="translate(., '&amp;#91;&amp;#93;[]&lt;>', '')"/>
  </xsl:template>
  <xsl:template match="caption/*/text()" mode="#all">
    <xsl:value-of select="translate(., '&amp;#91;&amp;#93;[]&lt;>', '')"/>
  </xsl:template>

  <!-- format ography tags as anchors to setup modal dialog boxes  HVN-->

  <xsl:template match="persName | placeName | name" mode="#all">
    <a>
      <xsl:call-template name="element_attributes"/>
      <xsl:choose>
        <xsl:when test="@ref or @data-ref">
          <xsl:attribute name="data-toggle">modal</xsl:attribute>
          <xsl:attribute name="data-target">#OgraphyModal</xsl:attribute>
        </xsl:when>
        <xsl:otherwise> </xsl:otherwise>
      </xsl:choose>
      <xsl:apply-templates/>
    </a>
    
  </xsl:template>


  <xsl:template match="list[@type = 'ordered']" mode="#all">
    <ol>
      <xsl:call-template name="element_attributes"/>
      <xsl:apply-templates mode="#current"/>
    </ol>
  </xsl:template>

  <xsl:template match="lg | list | listBibl" mode="#all">
    <ul>
      <xsl:call-template name="element_attributes"/>
      <xsl:apply-templates mode="#current"/>
    </ul>
  </xsl:template>

  <xsl:template match="*[parent::lg | parent::list | parent::listBibl]" mode="#all">
    <li>
      <xsl:call-template name="element_attributes"/>
      <xsl:apply-imports/>
    </li>
  </xsl:template>

  <xsl:template match="note[@place = 'footnote']" mode="inline">
    <a>
      <xsl:call-template name="element_attributes"/>
      <xsl:variable name="type">
        <xsl:choose>
          <xsl:when test="@n">original</xsl:when>
          <xsl:otherwise>position</xsl:otherwise>
        </xsl:choose>
      </xsl:variable>
      <xsl:variable name="number">
        <xsl:choose>
          <xsl:when test="@n">
            <xsl:value-of select="@n"/>
          </xsl:when>
        </xsl:choose>
      </xsl:variable>
      <xsl:variable name="href">
        <xsl:value-of select="@place"/>
        <xsl:text>--</xsl:text>
        <xsl:value-of select="$type"/>
        <xsl:text>--</xsl:text>
        <xsl:value-of select="$number"/>
      </xsl:variable>

      <xsl:attribute name="href">
        <xsl:text>#</xsl:text>
        <xsl:value-of select="$href"/>
      </xsl:attribute>
      <xsl:attribute name="id">
        <xsl:value-of select="$href"/>
        <xsl:text>--back</xsl:text>
      </xsl:attribute>
      <xsl:if test="$type = 'position'">
        <xsl:attribute name="title">Autonumbered</xsl:attribute>
      </xsl:if>

      <xsl:value-of select="$number"/>
    </a>
  </xsl:template>

  <xsl:template match="note[@place = 'footnote']" mode="footnotes">
    <dt>
      <a>
        <xsl:call-template name="element_attributes"/>
        <xsl:variable name="type">
          <xsl:choose>
            <xsl:when test="@n">original</xsl:when>
            <xsl:otherwise>position</xsl:otherwise>
          </xsl:choose>
        </xsl:variable>
        <xsl:variable name="number">
          <xsl:choose>
            <xsl:when test="@n">
              <xsl:value-of select="@n"/>
            </xsl:when>

          </xsl:choose>
        </xsl:variable>
        <xsl:variable name="href">
          <xsl:value-of select="@place"/>
          <xsl:text>--</xsl:text>
          <xsl:value-of select="$type"/>
          <xsl:text>--</xsl:text>
          <xsl:value-of select="$number"/>
        </xsl:variable>

        <xsl:attribute name="href">
          <xsl:text>#</xsl:text>
          <xsl:value-of select="$href"/>
          <xsl:text>--back</xsl:text>
        </xsl:attribute>
        <xsl:attribute name="id">
          <xsl:value-of select="$href"/>
        </xsl:attribute>
        <xsl:if test="$type = 'position'">
          <xsl:attribute name="title">Autonumbered</xsl:attribute>
        </xsl:if>

        <xsl:value-of select="$number"/>
      </a>
    </dt>
    <dd>
      <xsl:apply-imports/>
    </dd>
  </xsl:template>

  <xsl:template match="p" mode="#all">
    <!-- HTML does not allow for nested <p> tags. So only the element <p> should
         Be a <p> tag. -->
    <p>
      <xsl:call-template name="element_attributes"/>
      <xsl:apply-imports/>
    </p>
  </xsl:template>

  <xsl:template match="note[not(@place = 'footnote')]" mode="#all">
    <div class="note">
      Editorial Note:<xsl:apply-templates mode="note"/>
    </div>
  </xsl:template>

  <xsl:template match="*" mode="note">
    <xsl:value-of select="name(.)"/> : <xsl:apply-templates/>
  </xsl:template>

  <xsl:template match="gap" mode="#all">
    <span>
      <xsl:attribute name="class" select="'gap'"/>
      <xsl:text>[</xsl:text>
      <xsl:value-of select="@reason"/>
      <xsl:text>]</xsl:text>
    </span>
  </xsl:template>

  <xsl:template match="epigraph | label" mode="#all">
    <div>
      <xsl:call-template name="element_attributes"/>
      <xsl:apply-imports/>
    </div>
  </xsl:template>

  <xsl:template match="zone" mode="#all">
    <div>
      <xsl:call-template name="element_attributes"/>
      <xsl:apply-templates mode="#current"/>
    </div>
  </xsl:template>

  <xsl:template match="q | quote" mode="#all">
    <q>
      <xsl:call-template name="element_attributes"/>
      <xsl:apply-templates mode="#current"/>
    </q>
  </xsl:template>

  <xsl:template match="choice" mode="#all">

    <xsl:choose>
      <xsl:when test="reg | expan | corr">
        <xsl:apply-templates select="(reg | expan | corr)[1]" mode="#current"/>
      </xsl:when>
      <xsl:otherwise>
        <xsl:apply-templates select="*[1]" mode="#current"/>
      </xsl:otherwise>
    </xsl:choose>

  </xsl:template>

  <xsl:template name="element_attributes">
    <xsl:param name="node" select="current()"/>

    <xsl:attribute name="class">
      <xsl:value-of select="local-name($node)"/>
      <xsl:for-each select="$node/@type | $node/@rend | $node/@place | $node/@style | $node/@reason">
        <xsl:text> </xsl:text>
        <xsl:value-of
          select="concat('class--', local-name(.), '--', translate(normalize-space(.), ' ', '-'))"/>
      </xsl:for-each>
      <xsl:if test="$node/@spanTo">
        <xsl:text> spans spanTo--</xsl:text>
        <xsl:value-of select="generate-id(//*[@xml:id = substring($node/@spanTo, 2)])"/>
      </xsl:if>
      <xsl:if test="$node/@target">
        <xsl:text> targets</xsl:text>
        <xsl:for-each select="//*[@xml:id and contains($node/@target, concat('#', @xml:id))]">
          <xsl:text> target--</xsl:text>
          <xsl:value-of select="generate-id()"/>
        </xsl:for-each>
      </xsl:if>
    </xsl:attribute>
    <xsl:for-each select="$node/@*">
      <xsl:variable name="attribute_name">
        <xsl:variable name="prefix" select="substring-before(name(), ':')"/>
        <xsl:if test="$prefix">
          <xsl:text>-</xsl:text>
          <xsl:value-of select="$prefix"/>
        </xsl:if>
        <xsl:text>-</xsl:text>
        <xsl:value-of select="local-name()"/>
      </xsl:variable>
      <xsl:attribute name="data{$attribute_name}">
        <xsl:value-of select="."/>
      </xsl:attribute>
    </xsl:for-each>
  </xsl:template>

</xsl:stylesheet>
