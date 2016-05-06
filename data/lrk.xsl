<?xml version="1.0" encoding="Windows-1251"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:t="http://en.wikipedia.org/wiki/Uniform_resource_identifier" version="1.0">

  <xsl:output method ="html"/>

  <xsl:key name="symbols-key" match="//t:goto/t:symbol[not (.='$')]" use ="."/>

  <xsl:include href ="grammar.xsl"/>


  <xsl:template match ="/">
    <html>
      <head>
        <meta http-equiv="Content-Type" content="text/html; charset=windows-1251"/>
        <title>Таблица LR(k) анализатора</title>
      </head>
      <body>
        <xsl:apply-templates/>
      </body>
    </html>
  </xsl:template>

  <xsl:template match ="t:analyzer">
    
    <xsl:variable name="blocks-group" select="//t:goto/t:symbol[generate-id(.) = generate-id(key('symbols-key', .))]" />

    <p>
      <xsl:text>k = </xsl:text>
      <xsl:value-of select ="@k"/>
    </p>
    <p>
      <table style="background-color: transparent; width: 100%; border-style: none; padding: 0em; margin: 0 auto; border-spacing: 0px; border-collapse: collapse;">
        
        <tr>
          <th style="border: solid; border-width: 1px; border-color: Black; padding: 1px; margin: 0em auto; background-color: #B8FFC8; text-align: center; vertical-align: middle;">
            <xsl:text>State</xsl:text>
          </th>
          <th style="border: solid; border-width: 1px; border-color: Black; padding: 1px; margin: 0em auto; background-color: #B8FFC8; text-align: center; vertical-align: middle;">
            <xsl:attribute name ="colspan">
              <xsl:call-template name ="getActionCount">
                <xsl:with-param name ="nodes" select ="//t:action/t:string"/>
                <xsl:with-param name ="maxCount" select ="count(//t:action/t:string)"/>
                <xsl:with-param name ="currCount" select ="1"/>
              </xsl:call-template>
            </xsl:attribute>
            <xsl:text>Action</xsl:text>
            </th>
          <th style="border: solid; border-width: 1px; border-color: Black; padding: 1px; margin: 0em auto; background-color: #B8FFC8; text-align: center; vertical-align: middle;">
            <xsl:attribute name ="colspan">
              <xsl:value-of select="count($blocks-group)"/>
            </xsl:attribute>
            <xsl:text>Goto</xsl:text>
            </th>
        </tr>
        <tr>
          <th style="border: solid; border-width: 1px; border-color: Black; padding: 1px; margin: 0em auto; background-color: #B8FFC8; text-align: center; vertical-align: middle;">
            <xsl:text disable-output-escaping ="yes">&amp;nbsp;</xsl:text>
          </th>
          <xsl:for-each select ="//t:action/t:string">
            <xsl:variable name ="currValue">
              <xsl:call-template name ="getValue">
                <xsl:with-param name ="node" select ="."/>
              </xsl:call-template>
            </xsl:variable>

            <xsl:variable name ="isExist">
              <xsl:call-template name ="getIsExist">
                <xsl:with-param name ="pos" select ="position()-1"/>
                <xsl:with-param name ="currValue" select ="$currValue"/>
              </xsl:call-template>
                                
            </xsl:variable>


            <xsl:if test="$isExist='1'">
              <th style="border: solid; border-width: 1px; border-color: Black; padding: 1px; margin: 0em auto; background-color: #B8FFC8; text-align: center; vertical-align: middle;">
              <xsl:value-of select ="$currValue"/>
              </th>
            </xsl:if>



          </xsl:for-each>

          <xsl:for-each select ="$blocks-group">
            <th style="border: solid; border-width: 1px; border-color: Black; padding: 1px; margin: 0em auto; background-color: #B8FFC8; text-align: center; vertical-align: middle;">

              <xsl:value-of select ="."/>
            </th>
          </xsl:for-each>
          

        </tr>
        <xsl:for-each select ="t:state">

          <xsl:variable name="bgColor">
            <xsl:choose>
              <xsl:when test="((position()-1) mod 4) &gt; 1">#F0F0F0</xsl:when>
              <xsl:otherwise>#FFFFFF</xsl:otherwise>
            </xsl:choose>
          </xsl:variable>

          <xsl:variable name ="cNode" select ="."/>
          
          <tr>
            <xsl:attribute name="style">
              <xsl:text>background-color: </xsl:text>
              <xsl:value-of select="$bgColor"/>
            </xsl:attribute>
            
            <td style="border: solid; border-width: 1px; border-color: Black; padding: 1px; margin: 0em auto; text-align: center; vertical-align: middle;">
              <xsl:if test ="parent::node()/@startState=@name">
                <xsl:text disable-output-escaping="yes">&amp;gt;&amp;gt;</xsl:text>
              </xsl:if>
              <xsl:value-of select="@name"/>
            </td>

            <xsl:for-each select ="//t:action/t:string">
              <xsl:variable name ="currValue">
                <xsl:call-template name ="getValue">
                  <xsl:with-param name ="node" select ="."/>
                </xsl:call-template>
              </xsl:variable>

              <xsl:variable name ="isExist">
                <xsl:call-template name ="getIsExist">
                  <xsl:with-param name ="pos" select ="position()-1"/>
                  <xsl:with-param name ="currValue" select ="$currValue"/>
                </xsl:call-template>
              </xsl:variable>

              <xsl:if test="$isExist='1'">
                <td style="border: solid; border-width: 1px; border-color: Black; padding: 1px; margin: 0em auto; text-align: center; vertical-align: middle;">
                  <xsl:variable name ="isExistCurrent">
                    <xsl:call-template name ="getIsExistCurrent">
                      <xsl:with-param name ="currValue" select ="$currValue"/>
                      <xsl:with-param name ="currNodes" select ="$cNode"/>
                      <xsl:with-param name ="pos" select ="count($cNode/t:action)"/>
                    </xsl:call-template>
                  </xsl:variable>

                  <xsl:choose>
                    <xsl:when test ="$isExistCurrent=0">
                      <xsl:text disable-output-escaping="yes">&amp;nbsp;</xsl:text> 
                    </xsl:when>
                    <xsl:otherwise>
                      <xsl:for-each select ="$cNode/t:action">
                        <xsl:if test ="position()=$isExistCurrent">
                          <xsl:apply-templates select ="."/>
                        </xsl:if>
                      </xsl:for-each>
                    </xsl:otherwise>
                  </xsl:choose>
                  
                </td>
              </xsl:if>



            </xsl:for-each>

            <xsl:for-each select ="$blocks-group">
              <td style="border: solid; border-width: 1px; border-color: Black; padding: 1px; margin: 0em auto; text-align: center; vertical-align: middle;">
                <xsl:variable name ="cValue" select ="."/>
                
                <xsl:choose>
                  <xsl:when test ="$cNode/t:goto[t:symbol=$cValue]">
                    <xsl:value-of select ="$cNode/t:goto[t:symbol=$cValue]/t:value"/>
                  </xsl:when>
                  <xsl:otherwise>
                    <xsl:text disable-output-escaping="yes">&amp;nbsp;</xsl:text>
                  </xsl:otherwise>
                </xsl:choose>
              </td>
            </xsl:for-each>

          </tr>
        </xsl:for-each>
      </table>
    </p>
  </xsl:template>


  <xsl:template name ="getValueNode">
    <xsl:param name="node"/>
    <xsl:param name ="pos"/>

    <xsl:call-template name ="getValue">
      <xsl:with-param name ="node" select ="$node[$pos]"/>
    </xsl:call-template>
  </xsl:template>

  <xsl:template name="getValue">
    <xsl:param name="node"/>

    <xsl:for-each select ="$node/t:symbol">
      <xsl:sort  data-type ="number" select="@order"/>
      <xsl:value-of select ="."/>
    </xsl:for-each>
  </xsl:template>

  <xsl:template name ="getIsExist">
    <xsl:param name ="pos"/>
    <xsl:param name ="currValue"/>

    <xsl:choose>
      <xsl:when test="$pos=0">1</xsl:when>
      <xsl:otherwise>
        <xsl:variable name ="posValue">
          <xsl:call-template name ="getValueNode">
            <xsl:with-param name ="node" select ="//t:action/t:string"/>
            <xsl:with-param name ="pos" select="$pos"/>
          </xsl:call-template>
        </xsl:variable>
        
        <xsl:choose>
          <xsl:when test="$posValue=$currValue">0</xsl:when>
          <xsl:otherwise>
            <xsl:call-template name ="getIsExist">
              <xsl:with-param name ="pos" select ="($pos)-1"/>
              <xsl:with-param name ="currValue" select ="$currValue"/>
            </xsl:call-template>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name ="getIsExistCurrent">
    <xsl:param name ="currNodes"/>
    <xsl:param name ="currValue"/>
    <xsl:param name ="pos"/>


    <xsl:choose>
      <xsl:when test="$pos=0">0</xsl:when>
      <xsl:otherwise>
        <xsl:variable name ="posValue">
          <xsl:call-template name ="getValueNode">
            <xsl:with-param name ="node" select ="$currNodes/t:action/t:string"/>
            <xsl:with-param name ="pos" select="$pos"/>
          </xsl:call-template>
        </xsl:variable>

        <xsl:choose>
          <xsl:when test="$posValue=$currValue">
            <xsl:value-of select ="$pos"/>
          </xsl:when>
          <xsl:otherwise>
            <xsl:call-template name ="getIsExistCurrent">
              <xsl:with-param name ="pos" select ="($pos)-1"/>
              <xsl:with-param name ="currValue" select ="$currValue"/>
              <xsl:with-param name ="currNodes" select ="$currNodes"/>
            </xsl:call-template>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:otherwise>
    </xsl:choose>

  </xsl:template>


    <xsl:template name ="getActionCount">

      <xsl:param name ="nodes"/>
      <xsl:param name ="maxCount"/>
      <xsl:param name ="currCount"/>

      <xsl:variable name ="currValue">
        <xsl:call-template name ="getValue">
          <xsl:with-param name ="node" select ="$nodes[$currCount]"/>
        </xsl:call-template>
      </xsl:variable>
      
        <xsl:variable name ="isExist">
          <xsl:call-template name ="getIsExist">
            <xsl:with-param name ="pos" select ="($currCount)-1"/>
            <xsl:with-param name ="currValue" select ="$currValue"/>
          </xsl:call-template>
        </xsl:variable>
      
            
      <xsl:variable name ="actionCount">
        <xsl:choose>
          <xsl:when test ="$maxCount = $currCount">
            <xsl:value-of select ="$isExist"/>
          </xsl:when>
          <xsl:otherwise>
            <xsl:variable name ="nextCount">
              <xsl:call-template name ="getActionCount">
                <xsl:with-param name ="nodes" select ="$nodes"/>
                <xsl:with-param name ="maxCount" select ="$maxCount"/>
                <xsl:with-param name ="currCount" select ="($currCount)+1"/>
              </xsl:call-template>
            </xsl:variable>
            <xsl:value-of select ="($nextCount)+($isExist)"/>
          </xsl:otherwise>
        </xsl:choose>
        
      </xsl:variable>

      <xsl:value-of select ="$actionCount"/>  
    
  </xsl:template>

  <xsl:template match ="t:action">
    <xsl:value-of select ="@value"/>

    <xsl:if test="@value='reduce'">
      <br/>
      <xsl:variable name ="rule" select ="t:rule" />
      <xsl:apply-templates select ="//t:PR[@order=$rule]"/>
    </xsl:if>
    
  </xsl:template>
</xsl:stylesheet>


