<?xml version="1.0" encoding="Windows-1251"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:t="http://en.wikipedia.org/wiki/Uniform_resource_identifier" version="1.0">

  <xsl:output method ="html"/>

  <xsl:template match ="/">
    <html>
      <head>
        <meta http-equiv="Content-Type" content="text/html; charset=windows-1251"/>
        <title>Грамматика</title>
      </head>
      <body>
        <xsl:apply-templates select ="t:grammar"/>
      </body>
    </html>
  </xsl:template>

  <xsl:template match ="t:grammar">
    <p>
          <xsl:text>Терминальные символы: </xsl:text>
          <xsl:for-each select ="t:TS">
            <xsl:value-of select ="."/>
            <xsl:if test ="position()!=last()">
              <xsl:text>, </xsl:text>
            </xsl:if>
          </xsl:for-each>
        </p>
        <p>
          <xsl:text>Нетерминальные символы: </xsl:text>
          <xsl:for-each select ="t:NS">
            <xsl:value-of select ="."/>
            <xsl:if test ="position()!=last()">
              <xsl:text>, </xsl:text>
            </xsl:if>
          </xsl:for-each>
        </p>
        <p>
          <xsl:text>Аксиома: </xsl:text>
          <xsl:value-of select ="t:SS"/>
        </p>
        <p>
          <xsl:text>Правила грамматики: </xsl:text>
          <br/>
          <xsl:for-each select ="t:PR">
            <xsl:sort data-type ="number" select ="@order"/>
            <xsl:apply-templates select ="."/>
            <xsl:if test="position()!=last()">
              <br/>
            </xsl:if>
          </xsl:for-each>
        </p>


    
  </xsl:template>

  <xsl:template match="t:PR">
    <xsl:value-of select ="t:left/t:PRS[@order=1]"/>
    <xsl:text disable-output-escaping ="yes">&amp;nbsp;->&amp;nbsp;</xsl:text>
    <xsl:for-each select ="t:right/t:PRS">
      <xsl:sort data-type ="number" select ="@order"/>
      <xsl:value-of select ="."/>
    </xsl:for-each>
   
  </xsl:template>
  
</xsl:stylesheet>
