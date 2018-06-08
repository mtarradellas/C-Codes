<?xml version="1.0"?>
<xsl:stylesheet version="2.0"
 xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
 xmlns:fn="http://www.w3.org/2005/xpath-functions">
 <xsl:output method="xml" indent="yes"/>
 <xsl:strip-space elements="*"/>

 <xsl:variable name="map" select="document('myMap.xml')"/>

  <xsl:template match="/">
    <result>
      <xsl:apply-templates/>
    </result>
  </xsl:template>

  <xsl:template match="results">
    <xsl:apply-templates/>
  </xsl:template>

  <xsl:template match="results/conversation/user_history">
    <xsl:apply-templates/>
  </xsl:template>

  <xsl:template match="node()">
  <xsl:variable name ="node" select="name()"/>
  <xsl:variable name ="aux" select="$map/entry[@key= $node ]"/>
  <xsl:choose>
    <xsl:when test="$aux != '' ">
      <xsl:element name="{$aux}">
         <xsl:apply-templates select="@*"/>
         <xsl:apply-templates/>
      </xsl:element>
    </xsl:when>
    <xsl:otherwise>
      <xsl:copy>
        <xsl:apply-templates/>
      </xsl:copy>
    </xsl:otherwise>
  </xsl:choose>
 </xsl:template>

 
 <xsl:template match="@*">
  <xsl:variable name ="att" select="name()"/>
  <xsl:variable name ="aux" select="$map/entry[@key= $att ]"/>
  <xsl:choose>
    <xsl:when test="$aux != '' ">
      <xsl:attribute name="{$aux}">
<!--         <xsl:variable name ="content" select=""/>
 -->        <xsl:value-of select = "."/>
      </xsl:attribute>
    </xsl:when>
    <xsl:otherwise>
      <xsl:copy/>
    </xsl:otherwise>
  </xsl:choose>
</xsl:template>

</xsl:stylesheet>