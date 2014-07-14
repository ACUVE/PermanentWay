#ifndef ACUVE_KML_TEMPLATE_HPP
#define ACUVE_KML_TEMPLATE_HPP

static char const * const s_kml_head =
R"S(<?xml version="1.0" encoding="UTF-8"?>
<kml xmlns="http://earth.google.com/kml/2.2">
    <Document>
        <name>
            あばばばばばば
        </name>
        <visibility>1</visibility>
        <StyleMap id="StyleMap_PushToLogPoint">
            <Pair>
                <key>
                    normal
                </key>
                <Style>
                    <IconStyle>
                        <scale>0.5</scale>
                        <Icon>
                            <href>http://gps.wintec.tw/img/push_to_log.png</href>
                        </Icon>
                    </IconStyle>
                </Style>
            </Pair>
            <Pair>
                <key>
                    highlight
                </key>
                <Style>
                    <IconStyle>
                        <Icon>
                            <href>http://gps.wintec.tw/img/highlight.png</href>
                        </Icon>
                    </IconStyle>
                </Style>
            </Pair>
        </StyleMap>
        <StyleMap id="StyleMap_ConnectTrack">
            <Pair>
                <key>
                    normal
                </key>
                <Style>
                    <IconStyle>
                        <scale>0.5</scale>
                        <Icon>
                            <href>http://gps.wintec.tw/img/marker.png</href>
                        </Icon>
                    </IconStyle>
                </Style>
            </Pair>
            <Pair>
                <key>
                    highlight
                </key>
                <Style>
                    <IconStyle>
                        <Icon>
                            <href>http://gps.wintec.tw/img/highlight.png</href>
                        </Icon>
                    </IconStyle>
                </Style>
            </Pair>
        </StyleMap>
        <StyleMap id="StyleMap_StartTrackPoint">
            <Pair>
                <key>
                    normal
                </key>
                <Style>
                    <IconStyle>
                        <scale>0.7</scale>
                        <Icon>
                            <href>http://gps.wintec.tw/img/track_start.png</href>
                        </Icon>
                    </IconStyle>
                </Style>
            </Pair>
            <Pair>
                <key>
                    highlight
                </key>
                <Style>
                    <IconStyle>
                        <Icon>
                            <href>http://gps.wintec.tw/img/highlight.png</href>
                        </Icon>
                    </IconStyle>
                </Style>
            </Pair>
        </StyleMap>
        <StyleMap id="StyleMap_TrackPoint">
            <Pair>
                <key>
                    normal
                </key>
                <Style>
                    <IconStyle>
                        <scale>0.5</scale>
                        <Icon>
                            <href>http://gps.wintec.tw/img/track_start.png</href>
                        </Icon>
                    </IconStyle>
                </Style>
            </Pair>
            <Pair>
                <key>
                    highlight
                </key>
                <Style>
                    <IconStyle>
                        <Icon>
                            <href>http://gps.wintec.tw/img/highlight.png</href>
                        </Icon>
                    </IconStyle>
                </Style>
            </Pair>
        </StyleMap>
        <StyleMap id="StyleMap_EndTrackPoint">
            <Pair>
                <key>
                    normal
                </key>
                <Style>
                    <IconStyle>
                        <scale>0.5</scale>
                        <Icon>
                            <href>http://gps.wintec.tw/img/track_end.png</href>
                        </Icon>
                    </IconStyle>
                </Style>
            </Pair>
            <Pair>
                <key>
                    highlight
                </key>
                <Style>
                    <IconStyle>
                        <Icon>
                            <href>http://gps.wintec.tw/img/highlight.png</href>
                        </Icon>
                    </IconStyle>
                </Style>
            </Pair>
        </StyleMap>
        <StyleMap id="video">
            <Pair>
                <key>
                    normal
                </key>
                <Style>
                    <IconStyle>
                        <scale>0.9</scale>
                        <Icon>
                            <href>http://gps.wintec.tw/img/movies.png</href>
                        </Icon>
                    </IconStyle>
                </Style>
            </Pair>
            <Pair>
                <key>
                    highlight
                </key>
                <Style>
                    <IconStyle>
                        <Icon>
                            <href>http://gps.wintec.tw/img/highlight.png</href>
                        </Icon>
                    </IconStyle>
                </Style>
            </Pair>
        </StyleMap>
)S";

static char const * const s_kml_speedbar =
u8R"S(        <ScreenOverlay>
            <name>
                SpeedBar
            </name>
            <visibility>1</visibility>
            <Icon>
                <href>http://gps.wintec.tw/img/GESPDBAR.GIF</href>
            </Icon>
            <overlayXY x="0" y="0.1" xunits="fraction" yunits="fraction"/>
            <screenXY x="0" y="0.1" xunits="fraction" yunits="fraction"/>
            <size x="160" y="30" xunits="pixels" yunits="pixels"/>
        </ScreenOverlay>
)S";

static char const *const s_kml_folder_head =
u8R"S(        <Folder>
            <name>Track</name>
)S";

static char const *const s_kml_track_head =
u8R"S(            <Placemark>
                <name>
                    This is a test track
                </name>
                <Style>
                    <Icon>
                        <href>root://icon/palette-2.png</href>
                        <y>128</y><w>32</w><h>32</h>
                    </Icon>
                    <LineStyle>
                        <color>
                            ff7733cc
                        </color>
                        <width>5</width>
                    </LineStyle>
                </Style>
                <LineString>
                    <tessellate>1</tessellate>
                    <coordinates>
)S";

static char const * const s_kml_track_foot =
u8R"S(                    </coordinates>
                </LineString>
                <description>
                    This is test description
                </description>
            </Placemark>
)S";

static char const * const s_kml_folder_foot =
u8R"S(        </Folder>
)S";

static char const * const s_kml_foot =
u8R"S(    </Document>
</kml>
)S";

#endif
