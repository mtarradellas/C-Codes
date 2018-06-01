<?xml version="1.0"?>
<xsl:transform version="2.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
  <xsl:output method = "xml" indent = "yes" />
    <xsl:template match="/">
      <result>
        <xsl:apply-templates />
      </result>
    </xsl:template>

    <xsl:template match = "results/conversation">
      <conversation>
	<xsl:attribute name="x44" select="@id"/>
        <xsl:apply-templates />
      </conversation>
    </xsl:template>

    <xsl:template match = "user_history">
      <user_history>
        <xsl:apply-templates />
      </user_history>
    </xsl:template>

    <xsl:template match = "user">
      <xb5>
        <xsl:attribute name="x5f" select="@name"/>
        <xsl:apply-templates />
      </xb5>
    </xsl:template>

    <xsl:template match="iq">
      <x4a>
        <xsl:attribute name="x44" select="@id"/>
        <xsl:attribute name="xa7" select="@type"/>
        <xsl:attribute name="xa5" select="@to"/>
        <xsl:apply-templates />
      </x4a>
    </xsl:template>
      

    <xsl:template match = "message">
      <x59>
        <xsl:apply-templates />
      </x59>
    </xsl:template>

    <xsl:template match = "presence">
      <x78>
        <xsl:apply-templates />
      </x78>
    </xsl:template>








</xsl:transform>
