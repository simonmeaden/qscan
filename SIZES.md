A List osupported types with their enum selection values.

###ISO Sizes.

#### A Series
The dimensions of the A series paper sizes are defined by the ISO 216 international paper size standard. The A series was adopted in Europe in the 19th century, and is currently used all around the world, apart from in the USA and Canada. The most common paper size used in English speaking countries around the world is A4, which is 210mm x 297mm (8.27 inches x 11.7 inches). The largest sheet from the A series is the A0 size of paper. It has an area of 1m2, and the dimensions are 841mm x 1189mm. The A series uses an aspect ratio of 1:√2, and other sizes in the series are defined by folding the paper in half, parallel to its smaller sides. For example, cutting an A4 in half, will create an two A5 sheets, and so forth. Any size of brochure can be made using paper from the next larger size, for example A3 sheets are folded to make A4 brochures. The standard lengths and widths of the A series are rounded to the nearest millimetre.

 enum | Size Name | Size in mm
 --- | --- | ---
 ISO_4A0 | 4A0 | 2378 x 1682 
 ISO_2A0 | 2A0 | 1682 x 1189 
 ISO_A0  | A0  | 1189 x 841
 ISO_A0_Plus | A0+ |914 x 1292
 ISO_A1 |  A1  | 841 x 594
 ISO_A1_Plus | A1+ |609 x 914
 ISO_A2 | A2 | 594 x 420
 ISO_A3 | A3 | 420 x 297 
 ISO_A3_Plus | A3+ |329 x 483
 ISO_A4 | A4 |   297 x 210
 ISO_A5 | A5 | 210 x 148
 ISO_A6 | A6 | 148 x 105
 ISO_A7 | A7 | 105 x 74
 ISO_A8 | A8 | 74 x 52
 ISO_A9 | A9 | 52 x 37
 ISO_A10 | A10 | 37 x 26

#### B Series
The dimensions of the B series paper sizes are defined by the ISO 216 international paper size standard. The B series is not as common as the A series. It was created to provide paper sizes that were not included in the A series. The B series uses an aspect ratio of 1:√2. The area of the B series paper sheets is the geometric mean of the A series sheets. For example, B1 is between A0 and A1 in size. While the B series is less common in office use, it is more regularly used in other special situations, such as posters, books, envelopes and passports.

 enum | Size Name | Size in mm
 --- | --- | ---
 ISO_B0 | B0 | 1000 × 1414
 ISO_B1 | B1 | 707 × 1000
 ISO_B2 | B2 | 500 × 707
 ISO_B3 | B3 | 353 × 500
 ISO_B4 | B4 | 250 × 353
 ISO_B5 | B5 | 176 × 250
 ISO_B6 | B6 | 125 × 176
 ISO_B7 | B7 | 88 × 125
 ISO_B8 | B8 | 62 × 88
 ISO_B9 | B9 | 44 × 62
 ISO_B10 | B10 | 31 × 44 mm

#### C Series      
The dimensions of the C series sizes are defined by the ISO 269 paper size standard. The C series is most commonly used for envelopes. The area of C series paper is the geometric mean of the areas of the A and B series paper of the same number. For example, C4 has a bigger area than A4, but smaller area than B4. Therefore an A4 piece of paper will fit into a C4 envelope. The aspect ratio of C series envelopes is 1:√2, and this means that an A4 sheet of paper when folded in half, parallel to its smaller sides, will fit nicely into a C5 envelope. When it is folded twice, it will fit into a C6 envelope, and so on.
                    
 enum | Size Name | Size in mm
 --- | --- | ---
 ISO_C0 | C0 | 917 × 1297
 ISO_C1 | C1 | 648 × 917
 ISO_C2 | C2 | 458 × 648
 ISO_C3 | C3 | 324 × 458
 ISO_C4 | C4 | 229 × 324
 ISO_C5 | C5 | 162 × 229 
 ISO_C6 | C6 | 114 × 162 
 ISO_C7 | C7 | 81 × 114
 ISO_C8 | C8 | 57 × 81 
 ISO_C9 | C9 | 40 × 57 
 ISO_C10 | C10 | 28 × 40 
 
### Envelope Sizes
 enum | Size Name | Size in mm
  --- | --- | ---
 ISO_Envelope_DL |DL | 110 x 220
 ISO_Envelope_B4 | B4| 353 x 250
 ISO_Envelope_B5 | B5| 250 x 176
 ISO_Envelope_B6 | B6| 176 x 125
 ISO_Envelope_C0 | C0| 917 x 1297
 ISO_Envelope_C1 | C1| 648 x 914
 ISO_Envelope_C2 | C2| 458 x 648
 ISO_Envelope_C3 | C3| 324 x 458
 ISO_Envelope_C4 | C4| 229 x 324
 ISO_Envelope_C5 | C5| 162 x 229 
 ISO_Envelope_C6_C5 |C6/C5 | 229 x 114
 ISO_Envelope_C6 | C6| 114 x 162
 ISO_Envelope_C7_C6 |C7/C6 | 81 x 16
 ISO_Envelope_C7 | C7| 81 x 114
 ISO_Envelope_E4 | E4| 400 x 280

####Raw Untrimmed Sizes
The dimensions of the RAW series paper sizes are defined by the ISO 217:1995 paper size standard. The format consists of the RA series and the SRA series. RA stands for ‘Raw Format A’, and SRA stands for ‘Supplementary Raw Format A’. The untrimmed raw paper is used for commercial printing, and as it is slightly larger than the A series format, it allows the ink to bleed to the edge of the paper, before being cut to match the A format.

 enum | Size Name | Size in mm
 --- | --- | ---
RAW_RA0 | RA0 ||860 × 1220
RAW_RA1 |RA1 |610 × 860
RAW_RA2 |RA2 |430 × 610
RAW_RA3 | RA3|305 × 430
RAW_RA4 | RA4|215 × 305
RAW_SRA0 |SRA0|900 × 1280
RAW_SRA1 |SRA1 |640 × 900
RAW_SRA2 | SRA2|450 × 640
RAW_SRA3 | SRA3|320 × 450
RAW_SRA4 |SRA4 |225 × 320
RAW_SRA1_Plus | SRA1|660 × 920
RAW_SRA2_Plus | SRA2|480 × 650
RAW_SRA3_Plus | SRA3|320 × 460
RAW_SRA3_Plus_Plus | SRA3|320 × 464
RAW_A0U | A0U|880 × 1230
RAW_A1U |A1U |625 × 880
RAW_A2U |A2U |450 × 625
RAW_A3U | A3U|330 × 450
RAW_A4U |A4U |240 × 330

####Transitional Sizes
The transitional paper size formats consist of the PA Series and the F Series. The PA Series proposed to be included into the ISO 216 standard in 1975. They were rejected by the committee who decided that the number of standardised paper formats should be kept to a minimum. However this series is still used today, in particular PA4 (or L4). Its width is that of the ISO standard A4, and height of Canadian P4 (210mm x 280mm). The PA4 format can easily be printed on equipment designed for A4 or US Letter size, which is why it is used by many international magazines, and is a good solution as the format of presentation slides. PA4 is more of a page format than a paper size. It has a 4:3 aspect ratio when in landscape orientation.

In Southeast Asia, a common size used is the non-standard F4. The longer side is 330mm, which is the same as the British Foolscap. The shorter side is 210mm, which is the same as the ISO A4. The F4 is occasionally known as (metric) Foolscap or Folio.

 enum | Size Name | Size in mm
 --- | --- | ---
Transitional_PA0 | PA0|840 × 1120
Transitional_PA1 | PA1  |560 × 840
Transitional_PA2 | PA2|420 × 560
Transitional_PA3 | PA3|280 × 420
Transitional_PA4 | PA4|210 × 280
Transitional_PA5 | PA5|140 × 210
Transitional_PA6 | PA6|105 × 140
Transitional_PA7 | PA7|70 × 105
Transitional_PA8 | PA8 |52 × 70 
Transitional_PA9 |PA9 |35 × 52
Transitional_PA10 |PA10 |26 × 35
Transitional_F0 | F0|841 × 1321
Transitional_F1 | F1|660 × 841
Transitional_F2 | F2|420 × 660
Transitional_F3 | F3|330 × 420
Transitional_F4 | F4|210 × 330
Transitional_F5 | F5|165 × 210
Transitional_F6 | F6|105 × 165
Transitional_F7 | F7|82 × 105
Transitional_F8 |F8 |52 × 82
Transitional_F9 | F9|41 × 52
Transitional_F10 | F10|26 × 41

###US Sizes 
####Loose Sizes
Paper sizes in North America do not follow a logical system like the ISO standard does. They have their own system that they follow. This means that scaling the paper sizes is more difficult. The US Letter paper size is the most popular size used throughout the US. It is also commonly used in Chile and the Philippines.

 enum | Size Name | Size in mm
 --- | --- | ---
US_Half_Letter |Half letter |140 × 216
US_Letter | Letter|216 × 279
US_Legal | Legal|216 × 356
US_Junior_Legal |Junior Legal |127 × 203
US_Ledger | Ledger|432 × 279
US_Tabloid | Tabloid|279 × 432
US_Gov_Letter | Government Letter|203 × 267
US_Gov_Legal |Government Legal |216 × 330

####ANSI
The American National Standards Institute (ANSI) adopted ANSI/ASME Y14.1, which defined a series of paper sizes in 1996, based on the standard 8.5 inches x 11 inches (216mm x 279mm) Letter size, which was named ‘ANSI A’. This series is fairly similar to the ISO standard, in that if you cut a sheet in half, you will product two sheets of the next smaller size. Ledger/Tabloid is known as ‘ANSI B’. The most common and widely used size is ANSI A, also known as ‘Letter’.

 enum | Size Name | Size in mm
 --- | --- | ---
US_ANSI_A | ANSI A|216 × 279
US_ANSI_B |ANSI B |279 × 432
US_ANSI_C | ANSI C|432 × 559
US_ANSI_D |ANSI D |559 × 864
US_ANSI_E |ANSI E |864 × 1118

####Architectural Sizes
The Architectural series (ARCH) is used by architects in North America, and they prefer to use this series instead of ANSI, because the aspect ratios are ratios of smaller whole numbers (4:3 and 3:2). The ARCH series of paper sizes is defined in the ANSI/ASME Y14.1 standard. The ARCH sizes are commonly used by architects for their large format drawings.

 enum | Size Name | Size in mm
 --- | --- | ---
US_Arch_A | Arch A|229 × 305
US_Arch_B | Arch B|305 × 457
US_Arch_C |Arch C |457 × 610
US_Arch_D | Arch D|610 × 914
US_Arch_E | Arch E|914 × 1219
US_Arch_E1 |Arch E1 |762 × 1067
US_Arch_E2 |Arch E2 |660 × 965
US_Arch_E3 | Arch E3|686 × 991

####Envelopes
There are three main types of envelopes used in the US and they do not correspond to the ISO standard of paper sizes. They are known as Commercial, Announcement and Catalog. Other less known styles of envelope are Baronial, Booklet and Square. To identify the difference between a ISO size and the US size, a hyphen is often inserted between the letter and number. For example, A2 becomes A-2.

Commercial envelopes, also referred to as Office envelopes, are most commonly used in business situations as they are suitable for automated franking and filling. They are long and thin, with an aspect ration of between 1:1.6 and 1:2.2. The most well known commercial envelope is No.10, as it is able to fit Letter size paper folded three times, or Legal size paper folded four times parallel to the shorter side.

Announcement envelopes, also known as A series envelopes, are used for greeting cards, invitations and photographs. Their aspect ratio ranges between 1:1.3 to 1:1.6, which makes them more of a square shape, rather than long and thin like the Commercial envelopes. The most popular announcement envelopes are Lady Grey (A2) and Diplomat (A9).

Catalog envelopes are most commonly used for catalogs, brochures, and are made with a center seam to make them more durable.Their aspect ratio ranges between 1:1.3 and 1:1.5, making them very similar shape to the announcement envelopes.

 enum | Size Name | Size in mm
 --- | --- | ---
US_Envelope_6_14 |6¼ |152 × 89
US_Envelope_6_34 |6¾ |165 × 92
US_Envelope_7 |7 |172 × 95
US_Envelope_7_34_Monarch |7¾ Monarch |191 × 98
US_Envelope_8_58 |8⅝ |219 × 92
US_Envelope_9 |9 |225 × 98
US_Envelope_10 |10 |241 × 104
US_Envelope_11 |11|264 × 114
US_Envelope_12 |12 |279 × 12
US_Envelope_14 | 14|292 × 127
US_Envelope_16 |16 |305 × 152
US_Envelope_A1 | A1|92 × 130
US_Envelope_A2_Lady_Grey | A2 Lady Grey|146 × 111
US_Envelope_A4 | A4|159 × 108
US_Envelope_A6_Thompsons_Standard | A6 Thompson's Standard| 165 × 121
US_Envelope_A7_Besselheim |A7 Besselheim |184 × 133
US_Envelope_A8_Carrs |A8 Carr's|206 × 140
US_Envelope_A9_Diplomat | A9 Diplomat|222 × 146
US_Envelope_A10_Willow |A10 Willow |241 × 152
US_Envelope_A_Long |A Long| 225 × 98
US_Envelope_1 | 1|229 × 152
US_Envelope_1_34 | 1¾|241 × 152 
US_Envelope_3 | 3|254 × 178
US_Envelope_6 | 6|267 × 191
US_Envelope_8 | 8|286 × 210
US_Envelope_9_34 |9¾ |286 × 222
US_Envelope_10_12 | 10½|305 × 229
US_Envelope_12_12 |12½ |318 × 241 
US_Envelope_13_12 | 13½|330 × 254
US_Envelope_14_12 |14½ |368 × 292
US_Envelope_15 | 15|381 × 254
US_Envelope_15_12 |15½ |  	394 × 305
      
###Japanese Sizes
####JIS B Sizes
The Japanese Industrial Standards (JIS), defines two main series of paper sizes. They are the JIS A Series, and the JIS B Series. Both of the these series are widely available in Japan, as well as China and Taiwan. The JIS A Series is exactly the same as the ISO A Series, but with different tolerances. Both the JIS A Series and the JIS B Series have the same aspect ratio, but the area of the B Series paper is 1.5 times larger than the A Series. As well as the JIS A and B Series, there are a number of traditional Japanese paper sizes used mostly by printers. These include Shiroku-ban and Kiku, which are the most commonly used.

 enum | Size Name | Size in mm
 --- | --- | ---
JIS_B0 |B0|1030 × 1456
JIS_B1 | B1|728 × 1030
JIS_B2 | B2|515 × 728
JIS_B3 | B3|364 × 515
JIS_B4 | B4|257 × 364
JIS_B5 | B5|182 × 257
JIS_B6 | B6|128 × 182
JIS_B7 | B7|91 × 128
JIS_B8 | B8|64 × 91
JIS_B9 | B9|45 × 64
JIS_B10 |B10 |32 × 45
JIS_B11 |B11 |22 × 32
JIS_B12 |B12 |16 × 22

####Non-standard and Traditional paper sizes.


enum | Size Name | Size in mm
 --- | --- | ---
JIS_A_ban | A-ban|625 x 880
JIS_B_ban |B-ban |765 x 1085
Japanese_AB | AB|210 x 257
Japanese_B40 | B40|103 x 182
Japanese_Shikisen | Shikisen|84 x 148 
Japanese_A_Koban | A-Koban|608 x 866
Japanese_B_Koban | B-Koban|754 x 1047
Japanese_Shirokuban_4 |Shiroku ban 4 |264 × 379
Japanese_Shirokuban_5 | Shiroku ban 5|189 × 26
Japanese_Shirokuban_6 |Shiroku ban 6 |127 × 188
Japanese_Kiku_4 | Kiku 4|227 × 306
Japanese_Kiku_5 | Kiku 5|151 × 22
Japanese_Zenshi | Zenshi|457 x 560
Japanese_Han_Kiri |Han Kiri |356 x 432
Japanese_Yatsu_giri | Yatsu-giri|254 x 305
Japanese_Matsu_giri | Matsu-giri|203 x 254
Japanese_Yotsu_giri | Yotsu-giri|165 x 216
Japanese_Dai_Kyabine | Dai Kyabine|130 x 180
Japanese_Kyabine | Kyabine|120 x 165
Japanese_Nimai_gake | Nimai-ake|102 x 127
Japanese_Potsutokaado | Potsutokaado|89 x 140
Japanese_Dai_Tefuda |Dai efuda |90 x 130
Japanese_Saabisu | Saabisu|82 x 114
Japanese_Tefuda | Tefuda|76 x 112
Japanese_Dai_meishi |Dai meishi |65 x 90
Japanese_Kokusai_ban | Kokusai-ban|216 x 280
Japanese_Hyoujun_gata |Hyoujun-gata |177 x 250
Japanese_Oo_gata | Oo-gata|177 x 230
Japanese_Chuu_gata |Chuu-gata |162 x 210
Japanese_Ko_gata | Ko-gata|148 x 210
Japanese_Ippitsu_sen | Ippitsu-sen|82 x 185
Japanese_Hanshi |Hanshi |242 x 343
Japanese_Mino |Mino |273 x 394
Japanese_Oohousho |Oohousho|394 x 530
Japanese_Chuuhousho | Chuuhousho |364 x 500
Japanese_Kohousho | Kohousho|333 x 470
Japanese_Nisho_no_uchi |Nisho no uchi |333 x 485
Japanese_Kusuma | Kusuma|939 x 1757

#### Envelopes

enum | Size Name | Size in mm
 --- | --- | ---
Chou 1 | Chou 1 | 142 x 332
JIS Chou 2 | Chou 2 | 119 x 277
JIS Chou 3 | Chou 3| 120 x 235
Chou 31 |  Chou 31| 105 x 235
Chou 30 | Chou 30 | 92 x 235
Chou 40 |Chou 40| 90 x 225
JIS Chou 4 | Chou 4| 90 x 205
Kaku A3 | Kaku A3 | 320 x 440
Kaku 0 |  Kaku 0| 287 x 382
Kaku 1 |  Kaku 1| 270 x 382
JIS Kaku 2 |  Kaku 2| 240 x 332
Kaku Koku-sai A4 | Kaku Koku-sai A4 | 229 x 324
JIS Kaku 3 |  Kaku 3| 216 x 277
JIS Kaku 4 |  Kaku 4| 197 x 267
JIS Kaku 5 |  Kaku 5 |190 x 240
JIS Kaku 6 |  Kaku 6| 162 x 229
JIS Kaku 7 |  Kaku 7| 142 x 205
JIS Kaku 8 |  Kaku 8| 119 x 197
You 0 | You 0 (Non-JIS) | 197 x 136
JIS You 0 | You 0 | 235 x 120
JIS You 1 | You 1 | 173 x 118 *
JIS You 2 | You 2 | 162 x 114
JIS You 3 | You 3| 148 x 98
JIS You 4 |  You 4| 235 x 105
JIS You 5 |  You 5| 217 x 95
JIS You 6 |  You 6| 190 x 98
JIS You 7 | You 7 | 165 x 92

*There are two envelope sizes named You 1, 173mm x 118mm and 176mm x 120mm. The smaller size however is apparently the industry standard so that is the only one I have included.

###Chinese Sizes

 enum | Size Name | Size in mm
 --- | --- | ---
Chinese_D0 |D0 |764 x 1064
Chinese_D1 | D1|532 x 760
Chinese_D2 | D2|380 x 528
Chinese_D3 | D3|264 x 376
Chinese_D4 | D4|188 x 260
Chinese_D5 | D5|130 x 184
Chinese_D6 | D6|92 x 126
Chinese_RD0 | RD0|787 x 1092
Chinese_RD1 | RD1|546 x 787
Chinese_RD2 |RD2 |393 x 546
Chinese_RD3 | RD3|273 x 393 
Chinese_RD4 |RD4 |196 x 273
Chinese_RD5 |RD5 |136 x 196
Chinese_RD6 | RD6|98 x 136

###Canadian Sizes

 enum | Size Name | Size in mm
 --- | --- | ---
Canadian_P1 | P1|560 × 860
Canadian_P2 | P2|430 × 560
Canadian_P3 | P3|280 × 430
Canadian_P4 | P4|215 × 280 
Canadian_P5 | P5|140 × 215
Canadian_P6 | P6|107 × 140 

###Columbian Sizes

 enum | Size Name | Size in mm
 --- | --- | ---
Columbian_Carta | Carta|216 × 279
Columbian_Extra_Tabloide |Extra Tabloide |304 × 457*
Columbian_Oficio | Oficio|216 × 330
Columbian_1_8_Pliego |⅛ Pliego |250 × 350
Columbian_1_4_Pliego | ¼ Pliego|350 × 500
Columbian_1_2_Pliego | ½ Pliego|500 × 700
Columbian_Pliego | Pliego|700 × 1000

* Actual size is 304mm × 457.2mm but as I am only returning integers the returned size is only 457mm.

###French Sizes

 enum | Size Name | Size in mm
 --- | --- | ---
French_Cloche |Cloche |300 × 400
French_Pot_ecolier | Pot, écolier| 310 × 400
French_Telliere | Tellière|340 × 440
French_Couronne_ecriture | Couronne écriture|360 × 360
French_Couronne_edition | Couronne édition|370 × 470
French_Roberto |Roberto | 	390 × 500
French_Ecu | Écu| 	400 × 520
French_Coquille | Coquille|440 × 560
French_Carre | Carré| 	450 × 560
French_Cavalier |Cavalier | 	460 × 620
French_Demi_raisin | Demi-raisin| 	325 × 500
French_Raisin |Raisin | 	500 × 650
French_Double_Raisin |Double Raisin | 	650 × 1000
French_Jesus |Jésus | 	560 × 760
French_Soliel | Soleil| 	600 × 800
French_Colombier_Affiche | Colombier affiche| 	600 × 800
French_Colombier_Commercial | Colombier commercial|630 × 900
French_Petit_Aigle | Petit Aigle| 	700 × 940 
French_Grand_Aigle | Grand Aigle| 	750 × 1050
French_Grand_Monde | Grand Monde| 	900 × 1260
French_Univers | Univers| 	1000 × 1130

###British Traditional and Transitional Sizes.
When the United Kingdom started using ISO sizes, the traditional paper sizes were no longer commonly used. Most of the traditional sizes were used in the production of books, and were never used for other stationery purposes. ‘Folio’ or ‘Foolscap’ is an alias for Foolscap Folio, as is ‘Kings’ being an alias for ‘Foolscap Quarto’.

 enum | Size Name | Size in mm
 --- | --- | ---
British_Dukes |Dukes| 	140 × 178
British_Foolscap | Foolscap | 	203 × 330
British_Imperial |Imperial | 	178 × 229
British_Kings |Kings | 	165 × 203
British_Quarto | Quarto|203 × 254

Before the United Kingdom adopted the ISO 216 standard, British Imperial paper sizes were used. The Imperial paper sizes were used to define large sheets of paper, and the naming convention was derived from the sheet name, and how many times it was folded. For example ‘Royal’ paper is 508mm x 635mm, and when it is folded three times, it makes 8 sheets and goes by the name ‘Royal Octavo’ which is 253 mm x 158 mm. This is a name given to modern hardbound books.

 enum | Size Name | Size in mm
 --- | --- | ---
Imperial_Antiquarian |Antiquarian | 	787 × 1346
Imperial_Atlas |Atlas | 	660 × 864
Imperial_Brief |Brief | 	343 × 406
Imperial_Broadsheet | Broadsheet| 	457 × 610
Imperial_Cartridge | Cartridge|533 × 660
Imperial_Columbier |Columbier |597 × 876
Imperial_Copy_Draught | Copy Draught|406 × 508
Imperial_Crown | Crown| 	381 × 508
Imperial_Demy | Demy| 	445 × 572
Imperial_Double_Demy |Double Demy |572 × 902
Imperial_Quad_Demy | Quad Demy|889 × 1143
Imperial_Elephant | Elephant| 	584 × 711 
Imperial_Double_Elephant |Double Elephant | 	678 × 1016
Imperial_Emperor | Emperor|1219 × 1829
Imperial_Foolscap | Foolscap| 	343 × 432
Imperial_Small_Foolscap |Small Foolscap |337 × 419
Imperial_Grand_Eagle |Grand Eagle |730 × 1067
Imperial_Imperial | Imperial|559 × 762
Imperial_Medium | Medium| 	470 × 584
Imperial_Monarch | Monarch| 	184 × 267 
Imperial_Post | Post|394 × 489 
Imperial_Sheet_Half_Post |Sheet, Half Post |495 × 597
Imperial_Pinched_Post |Pinched Post |375 × 470
Imperial_Large_Post |Large Post |394 × 508
Imperial_Double_Large_Post |Double Large Post | 	533 × 838
Imperial_Double_Post | Double Post| 	483 × 762
Imperial_Pott |Pott | 	318 × 381
Imperial_Princess |Princess | 	546 × 711
Imperial_Quarto |Quarto |229 × 279
Imperial_Royal |Royal | 	508 × 635
Imperial_Super_Royal |Super Royal | 	483 × 686


          
