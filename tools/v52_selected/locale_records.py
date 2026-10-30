# Selected V5.2 controls (RC3 + RC6 rows; RC7: caustics removed; RC8: tree distance removed) and eight translated, optically-padded native separators.
RECORDS = {'ar': {'cr01_pt_scene_range': 'PT: Scene Range',
        'option_cr01_pt_scene_range': 'PT: Scene Range',
        'option_urt52_sep_cpu': ' CPU\xa0',
        'option_urt52_sep_fg': ' Frame Generation\xa0',
        'option_urt52_sep_pt': ' Path Tracing\xa0',
        'option_urt52_sep_rt': ' Ray Tracing\xa0',
        'option_urt52_sep_rtgip': ' RTGI: الأداء\xa0',
        'option_urt52_sep_rtgiq': ' RTGI: الجودة\xa0',
        'option_urt52_sep_rtpt': ' RT/PT: مشترك\xa0',
        'option_urt52_sep_tex': ' الخامات / العرض\xa0',
        'option_urtx_fgmenuresources': 'Frame Generation: Keep Resources in Menus',
        'option_urtx_ptaoquality': 'PT: Ambient Occlusion Quality',
        'option_urtx_ptskinbouncedesat': 'PT: Skin Bounce Light Desaturation',
        'option_urtx_rtaopretrace': 'RT: Ambient Occlusion Pre-Trace',
        'option_urtx_rtaoscreenspace': 'RT: Ambient Occlusion Screen-Space Sampling',
        'option_urtx_rthbil': 'RT: Screen-Space Indirect Lighting (HBIL)',
        'option_urtx_rtreflfog': 'RT/PT: Fog in Ray-Traced Reflections',
        'option_urtx_rtreflscreenspace': 'RT: Screen-Space Reflection Assist',
        'option_urtx_terrainpreload': 'Terrain Preload Distance',
        'option_urtx_texturestreams': 'Textures Streamed at Once',
        'preset_value_urt_m150': '150 m',
        'preset_value_urt_m200': '200 m',
        'preset_value_urt_m250': '250 m',
        'preset_value_urt_m300': '300 m',
        'tooltip_cr01_pt_scene_range': 'يصغّر منطقة تتبع الأشعة حولك (اللعبة: 400 م)؛ قد تضعف الانعكاسات والإضاءة غير '
                                       'المباشرة والظلال البعيدة. القيم الأقل قد تحسن FPS. Path Tracing فقط. يُطبق '
                                       'فورًا.',
        'tooltip_urtx_fgmenuresources': 'يحتفظ بموارد Frame Generation أثناء فتح قائمة بملء الشاشة، مما قد يتجنب '
                                        'التقطع عند العودة. لا يتغير أي إعداد آخر لـ Frame Generation أو Reflex أو '
                                        'V-Sync. يستخدم ذاكرة فيديو إضافية أثناء فتح القائمة؛ لا يغير FPS أثناء اللعب. '
                                        'يُطبق فورًا.',
        'tooltip_urtx_ptaoquality': 'جودة الحجب المحيطي في تتبع المسار؛ المستويات الأدنى أكثر تشويشًا. القيم الأقل قد '
                                    'تحسن FPS. Path Tracing فقط. يُطبق فورًا.',
        'tooltip_urtx_ptskinbouncedesat': 'يزيل اللون من الضوء المرتد على الجلد؛ القيم الأعلى تعطي ألوان بشرة أكثر '
                                          'حيادية. تفضيل بصري؛ لا توجد فائدة مثبتة في FPS. Path Tracing فقط. يُطبق '
                                          'فورًا.',
        'tooltip_urtx_rtaopretrace': 'يشغّل مرحلة التتبع المسبق للحجب المحيطي بتتبع الأشعة. يعتمد تأثيره في FPS على '
                                     'المشهد. تتبع الأشعة العادي فقط. يُطبق فورًا.',
        'tooltip_urtx_rtaoscreenspace': 'يسمح للانسداد المحيطي بتتبع الأشعة باستخدام عينات الشاشة (التشغيل افتراضيًا). '
                                        'الإيقاف يستخدم الأشعة فقط. يعتمد تأثيره في FPS على المشهد. تتبع الأشعة العادي '
                                        'فقط. يُطبق فورًا.',
        'tooltip_urtx_rthbil': 'يضيف ضوءًا غير مباشر من فضاء الشاشة (HBIL) إلى الإضاءة الشاملة بتتبع الأشعة. الإيقاف '
                               'قد يحسن FPS. تتبع الأشعة العادي فقط. يُطبق فورًا.',
        'tooltip_urtx_rtreflfog': 'يطبّق الضباب داخل انعكاسات تتبع الأشعة. الإيقاف يعطي انعكاسات أوضح في الضباب ويوفر '
                                  'قليلًا من وقت GPU. الإيقاف قد يحسن FPS. تتبع الأشعة العادي و Path Tracing. يُطبق '
                                  'فورًا.',
        'tooltip_urtx_rtreflscreenspace': 'يسمح للانعكاسات باستخدام بيانات فضاء الشاشة قبل تتبع الأشعة. التشغيل قد '
                                          'يحسن FPS. تتبع الأشعة العادي فقط. يُطبق فورًا.',
        'tooltip_urtx_terrainpreload': 'إلى أي مدى تُحمَّل التضاريس مسبقًا في اتجاه حركتك (الأصلي 20 m). القيم الأعلى '
                                       'تقلل ظهور العناصر المفاجئ لكنها تزيد عمل التحميل. يُطبق على المناطق المحملة '
                                       'حديثًا.',
        'tooltip_urtx_texturestreams': 'عدد الخامات التي تُحمَّل في الوقت نفسه (الأصلي 12). القيم الأعلى تقلل ظهور '
                                       'العناصر المفاجئ لكنها تزيد عمل التحميل. يُطبق فورًا.',
        'urt52_sep_cpu': ' CPU\xa0',
        'urt52_sep_fg': ' Frame Generation\xa0',
        'urt52_sep_pt': ' Path Tracing\xa0',
        'urt52_sep_rt': ' Ray Tracing\xa0',
        'urt52_sep_rtgip': ' RTGI: الأداء\xa0',
        'urt52_sep_rtgiq': ' RTGI: الجودة\xa0',
        'urt52_sep_rtpt': ' RT/PT: مشترك\xa0',
        'urt52_sep_tex': ' الخامات / العرض\xa0',
        'urtx_fgmenuresources': 'Frame Generation: Keep Resources in Menus',
        'urtx_n_0': '0',
        'urtx_n_0p005': '0.005',
        'urtx_n_0p015': '0.015',
        'urtx_n_0p02': '0.02',
        'urtx_n_0p03': '0.03',
        'urtx_n_0p05': '0.05',
        'urtx_n_0p1': '0.1',
        'urtx_n_0p25': '0.25',
        'urtx_n_0p5': '0.5',
        'urtx_n_0p75': '0.75',
        'urtx_n_1': '1',
        'urtx_n_10000_mb': '10000 MB',
        'urtx_n_100_m': '100 m',
        'urtx_n_10_m': '10 m',
        'urtx_n_16': '16',
        'urtx_n_1600_mb': '1600 MB',
        'urtx_n_2': '2',
        'urtx_n_2000_mb': '2000 MB',
        'urtx_n_200_m': '200 m',
        'urtx_n_24': '24',
        'urtx_n_3': '3',
        'urtx_n_3000_mb': '3000 MB',
        'urtx_n_32': '32',
        'urtx_n_4000_mb': '4000 MB',
        'urtx_n_400_m': '400 m',
        'urtx_n_40_m': '40 m',
        'urtx_n_48': '48',
        'urtx_n_6000_mb': '6000 MB',
        'urtx_n_600_m': '600 m',
        'urtx_n_64': '64',
        'urtx_n_8': '8',
        'urtx_n_8000_mb': '8000 MB',
        'urtx_n_minus0p5': '-0.5',
        'urtx_n_minus1': '-1',
        'urtx_n_minus1p5': '-1.5',
        'urtx_n_minus2': '-2',
        'urtx_ptaoquality': 'PT: Ambient Occlusion Quality',
        'urtx_ptskinbouncedesat': 'PT: Skin Bounce Light Desaturation',
        'urtx_rtaopretrace': 'RT: Ambient Occlusion Pre-Trace',
        'urtx_rtaoscreenspace': 'RT: Ambient Occlusion Screen-Space Sampling',
        'urtx_rthbil': 'RT: Screen-Space Indirect Lighting (HBIL)',
        'urtx_rtreflfog': 'RT/PT: Fog in Ray-Traced Reflections',
        'urtx_rtreflscreenspace': 'RT: Screen-Space Reflection Assist',
        'urtx_terrainpreload': 'Terrain Preload Distance',
        'urtx_texturestreams': 'Textures Streamed at Once',
        'urtx_w_keep': 'الاحتفاظ بالموارد',
        'urtx_w_off': 'إيقاف',
        'urtx_w_on': 'تشغيل'},
 'br': {'cr01_pt_scene_range': 'PT: Scene Range',
        'option_cr01_pt_scene_range': 'PT: Scene Range',
        'option_urt52_sep_cpu': ' CPU\xa0',
        'option_urt52_sep_fg': ' Frame Generation\xa0',
        'option_urt52_sep_pt': ' Path Tracing\xa0',
        'option_urt52_sep_rt': ' Ray Tracing\xa0',
        'option_urt52_sep_rtgip': ' RTGI: desempenho\xa0',
        'option_urt52_sep_rtgiq': ' RTGI: qualidade\xa0',
        'option_urt52_sep_rtpt': ' RT/PT: compartilhado\xa0',
        'option_urt52_sep_tex': ' Texturas / Renderização\xa0',
        'option_urtx_fgmenuresources': 'Frame Generation: Keep Resources in Menus',
        'option_urtx_ptaoquality': 'PT: Ambient Occlusion Quality',
        'option_urtx_ptskinbouncedesat': 'PT: Skin Bounce Light Desaturation',
        'option_urtx_rtaopretrace': 'RT: Ambient Occlusion Pre-Trace',
        'option_urtx_rtaoscreenspace': 'RT: Ambient Occlusion Screen-Space Sampling',
        'option_urtx_rthbil': 'RT: Screen-Space Indirect Lighting (HBIL)',
        'option_urtx_rtreflfog': 'RT/PT: Fog in Ray-Traced Reflections',
        'option_urtx_rtreflscreenspace': 'RT: Screen-Space Reflection Assist',
        'option_urtx_terrainpreload': 'Terrain Preload Distance',
        'option_urtx_texturestreams': 'Textures Streamed at Once',
        'preset_value_urt_m150': '150 m',
        'preset_value_urt_m200': '200 m',
        'preset_value_urt_m250': '250 m',
        'preset_value_urt_m300': '300 m',
        'tooltip_cr01_pt_scene_range': 'Reduz a área com ray tracing ao seu redor (jogo: 400 m); reflexos, luz '
                                       'indireta e sombras distantes podem diminuir. Valores menores podem aumentar o '
                                       'FPS. Apenas Path Tracing. Aplica-se imediatamente.',
        'tooltip_urtx_fgmenuresources': 'Mantém os recursos do Frame Generation enquanto um menu em tela cheia está '
                                        'aberto, o que pode evitar um travamento ao voltar. Nenhum outro ajuste de '
                                        'Frame Generation, Reflex ou V-Sync é alterado. Usa mais memória de vídeo com '
                                        'um menu aberto; não muda o FPS durante o jogo. Aplica-se imediatamente.',
        'tooltip_urtx_ptaoquality': 'Qualidade da oclusão ambiente com path tracing; níveis baixos têm mais ruído. '
                                    'Valores menores podem aumentar o FPS. Apenas Path Tracing. Aplica-se '
                                    'imediatamente.',
        'tooltip_urtx_ptskinbouncedesat': 'Remove a cor da luz que rebate na pele; valores maiores dão tons de pele '
                                          'mais neutros. Preferência visual; sem ganho de FPS comprovado. Apenas Path '
                                          'Tracing. Aplica-se imediatamente.',
        'tooltip_urtx_rtaopretrace': 'Executa a passada de pré-traçado da oclusão ambiente com ray tracing. O efeito '
                                     'no FPS depende da cena. Apenas Ray Tracing normal. Aplica-se imediatamente.',
        'tooltip_urtx_rtaoscreenspace': 'Permite que a oclusão ambiente com ray tracing use amostras da tela (ligado '
                                        'por padrão). Desligado usa somente raios. O efeito no FPS depende da cena. '
                                        'Apenas Ray Tracing normal. Aplica-se imediatamente.',
        'tooltip_urtx_rthbil': 'Adiciona luz indireta em espaço de tela (HBIL) à iluminação global com ray tracing. '
                               'Desligado pode aumentar o FPS. Apenas Ray Tracing normal. Aplica-se imediatamente.',
        'tooltip_urtx_rtreflfog': 'Aplica névoa dentro dos reflexos com ray tracing. Desligado dá reflexos mais limpos '
                                  'com neblina e economiza um pouco de tempo de GPU. Desligado pode aumentar o FPS. '
                                  'Ray Tracing normal e Path Tracing. Aplica-se imediatamente.',
        'tooltip_urtx_rtreflscreenspace': 'Permite que os reflexos usem dados do espaço de tela antes de traçar raios. '
                                          'Ligado pode aumentar o FPS. Apenas Ray Tracing normal. Aplica-se '
                                          'imediatamente.',
        'tooltip_urtx_terrainpreload': 'Até onde o terreno é carregado antecipadamente na sua direção de deslocamento '
                                       '(nativo 20 m). Valores maiores reduzem o pop-in, mas aumentam o carregamento. '
                                       'Aplica-se às áreas recém-carregadas.',
        'tooltip_urtx_texturestreams': 'Número de texturas carregadas ao mesmo tempo (nativo 12). Valores maiores '
                                       'reduzem o pop-in, mas aumentam o carregamento. Aplica-se imediatamente.',
        'urt52_sep_cpu': ' CPU\xa0',
        'urt52_sep_fg': ' Frame Generation\xa0',
        'urt52_sep_pt': ' Path Tracing\xa0',
        'urt52_sep_rt': ' Ray Tracing\xa0',
        'urt52_sep_rtgip': ' RTGI: desempenho\xa0',
        'urt52_sep_rtgiq': ' RTGI: qualidade\xa0',
        'urt52_sep_rtpt': ' RT/PT: compartilhado\xa0',
        'urt52_sep_tex': ' Texturas / Renderização\xa0',
        'urtx_fgmenuresources': 'Frame Generation: Keep Resources in Menus',
        'urtx_n_0': '0',
        'urtx_n_0p005': '0.005',
        'urtx_n_0p015': '0.015',
        'urtx_n_0p02': '0.02',
        'urtx_n_0p03': '0.03',
        'urtx_n_0p05': '0.05',
        'urtx_n_0p1': '0.1',
        'urtx_n_0p25': '0.25',
        'urtx_n_0p5': '0.5',
        'urtx_n_0p75': '0.75',
        'urtx_n_1': '1',
        'urtx_n_10000_mb': '10000 MB',
        'urtx_n_100_m': '100 m',
        'urtx_n_10_m': '10 m',
        'urtx_n_16': '16',
        'urtx_n_1600_mb': '1600 MB',
        'urtx_n_2': '2',
        'urtx_n_2000_mb': '2000 MB',
        'urtx_n_200_m': '200 m',
        'urtx_n_24': '24',
        'urtx_n_3': '3',
        'urtx_n_3000_mb': '3000 MB',
        'urtx_n_32': '32',
        'urtx_n_4000_mb': '4000 MB',
        'urtx_n_400_m': '400 m',
        'urtx_n_40_m': '40 m',
        'urtx_n_48': '48',
        'urtx_n_6000_mb': '6000 MB',
        'urtx_n_600_m': '600 m',
        'urtx_n_64': '64',
        'urtx_n_8': '8',
        'urtx_n_8000_mb': '8000 MB',
        'urtx_n_minus0p5': '-0.5',
        'urtx_n_minus1': '-1',
        'urtx_n_minus1p5': '-1.5',
        'urtx_n_minus2': '-2',
        'urtx_ptaoquality': 'PT: Ambient Occlusion Quality',
        'urtx_ptskinbouncedesat': 'PT: Skin Bounce Light Desaturation',
        'urtx_rtaopretrace': 'RT: Ambient Occlusion Pre-Trace',
        'urtx_rtaoscreenspace': 'RT: Ambient Occlusion Screen-Space Sampling',
        'urtx_rthbil': 'RT: Screen-Space Indirect Lighting (HBIL)',
        'urtx_rtreflfog': 'RT/PT: Fog in Ray-Traced Reflections',
        'urtx_rtreflscreenspace': 'RT: Screen-Space Reflection Assist',
        'urtx_terrainpreload': 'Terrain Preload Distance',
        'urtx_texturestreams': 'Textures Streamed at Once',
        'urtx_w_keep': 'Manter recursos',
        'urtx_w_off': 'Desligado',
        'urtx_w_on': 'Ligado'},
 'cn': {'cr01_pt_scene_range': 'PT: Scene Range',
        'option_cr01_pt_scene_range': 'PT: Scene Range',
        'option_urt52_sep_cpu': ' CPU\xa0',
        'option_urt52_sep_fg': ' Frame Generation\xa0',
        'option_urt52_sep_pt': ' Path Tracing\xa0',
        'option_urt52_sep_rt': ' Ray Tracing\xa0',
        'option_urt52_sep_rtgip': ' RTGI：性能\xa0',
        'option_urt52_sep_rtgiq': ' RTGI：质量\xa0',
        'option_urt52_sep_rtpt': ' RT/PT：共用\xa0',
        'option_urt52_sep_tex': ' 纹理 / 渲染\xa0',
        'option_urtx_fgmenuresources': 'Frame Generation: Keep Resources in Menus',
        'option_urtx_ptaoquality': 'PT: Ambient Occlusion Quality',
        'option_urtx_ptskinbouncedesat': 'PT: Skin Bounce Light Desaturation',
        'option_urtx_rtaopretrace': 'RT: Ambient Occlusion Pre-Trace',
        'option_urtx_rtaoscreenspace': 'RT: Ambient Occlusion Screen-Space Sampling',
        'option_urtx_rthbil': 'RT: Screen-Space Indirect Lighting (HBIL)',
        'option_urtx_rtreflfog': 'RT/PT: Fog in Ray-Traced Reflections',
        'option_urtx_rtreflscreenspace': 'RT: Screen-Space Reflection Assist',
        'option_urtx_terrainpreload': 'Terrain Preload Distance',
        'option_urtx_texturestreams': 'Textures Streamed at Once',
        'preset_value_urt_m150': '150 m',
        'preset_value_urt_m200': '200 m',
        'preset_value_urt_m250': '250 m',
        'preset_value_urt_m300': '300 m',
        'tooltip_cr01_pt_scene_range': '缩小你周围的光线追踪范围（游戏默认：400 m）；远处的反射、间接光和阴影可能减弱。降低数值可能提升 FPS。仅限 Path Tracing。立即生效。',
        'tooltip_urtx_fgmenuresources': '在全屏菜单打开期间保留 Frame Generation 资源，可能避免返回游戏时的卡顿。不会更改 Frame Generation、Reflex 或 '
                                        'V-Sync 的其他设置。菜单打开时占用更多显存；游戏中 FPS 不变。立即生效。',
        'tooltip_urtx_ptaoquality': '路径追踪环境光遮蔽的质量；级别越低噪点越多。降低数值可能提升 FPS。仅限 Path Tracing。立即生效。',
        'tooltip_urtx_ptskinbouncedesat': '去除反弹到皮肤上的光线颜色；数值越高肤色越中性。属于视觉偏好，没有已确认的 FPS 收益。仅限 Path Tracing。立即生效。',
        'tooltip_urtx_rtaopretrace': '运行光线追踪环境光遮蔽的预追踪。对 FPS 的影响取决于场景。仅限普通 Ray Tracing。立即生效。',
        'tooltip_urtx_rtaoscreenspace': '允许光线追踪环境光遮蔽使用屏幕采样（默认开）。关则只使用光线。对 FPS 的影响取决于场景。仅限普通 Ray Tracing。立即生效。',
        'tooltip_urtx_rthbil': '在光线追踪全局光照上叠加屏幕空间间接光（HBIL）。关闭可能提升 FPS。仅限普通 Ray Tracing。立即生效。',
        'tooltip_urtx_rtreflfog': '在光线追踪反射中应用雾。关会让雾天的反射更清晰，并节省少量 GPU 时间。关闭可能提升 FPS。普通 Ray Tracing 与 Path Tracing。立即生效。',
        'tooltip_urtx_rtreflscreenspace': '在追踪光线前让反射利用屏幕空间数据。开启可能提升 FPS。仅限普通 Ray Tracing。立即生效。',
        'tooltip_urtx_terrainpreload': '沿行进方向提前加载地形的距离（原生 20 m）。提高数值可减少物体突然出现，但会增加加载工作。对新加载的区域生效。',
        'tooltip_urtx_texturestreams': '同时加载的纹理数量（原生 12）。提高数值可减少物体突然出现，但会增加加载工作。立即生效。',
        'urt52_sep_cpu': ' CPU\xa0',
        'urt52_sep_fg': ' Frame Generation\xa0',
        'urt52_sep_pt': ' Path Tracing\xa0',
        'urt52_sep_rt': ' Ray Tracing\xa0',
        'urt52_sep_rtgip': ' RTGI：性能\xa0',
        'urt52_sep_rtgiq': ' RTGI：质量\xa0',
        'urt52_sep_rtpt': ' RT/PT：共用\xa0',
        'urt52_sep_tex': ' 纹理 / 渲染\xa0',
        'urtx_fgmenuresources': 'Frame Generation: Keep Resources in Menus',
        'urtx_n_0': '0',
        'urtx_n_0p005': '0.005',
        'urtx_n_0p015': '0.015',
        'urtx_n_0p02': '0.02',
        'urtx_n_0p03': '0.03',
        'urtx_n_0p05': '0.05',
        'urtx_n_0p1': '0.1',
        'urtx_n_0p25': '0.25',
        'urtx_n_0p5': '0.5',
        'urtx_n_0p75': '0.75',
        'urtx_n_1': '1',
        'urtx_n_10000_mb': '10000 MB',
        'urtx_n_100_m': '100 m',
        'urtx_n_10_m': '10 m',
        'urtx_n_16': '16',
        'urtx_n_1600_mb': '1600 MB',
        'urtx_n_2': '2',
        'urtx_n_2000_mb': '2000 MB',
        'urtx_n_200_m': '200 m',
        'urtx_n_24': '24',
        'urtx_n_3': '3',
        'urtx_n_3000_mb': '3000 MB',
        'urtx_n_32': '32',
        'urtx_n_4000_mb': '4000 MB',
        'urtx_n_400_m': '400 m',
        'urtx_n_40_m': '40 m',
        'urtx_n_48': '48',
        'urtx_n_6000_mb': '6000 MB',
        'urtx_n_600_m': '600 m',
        'urtx_n_64': '64',
        'urtx_n_8': '8',
        'urtx_n_8000_mb': '8000 MB',
        'urtx_n_minus0p5': '-0.5',
        'urtx_n_minus1': '-1',
        'urtx_n_minus1p5': '-1.5',
        'urtx_n_minus2': '-2',
        'urtx_ptaoquality': 'PT: Ambient Occlusion Quality',
        'urtx_ptskinbouncedesat': 'PT: Skin Bounce Light Desaturation',
        'urtx_rtaopretrace': 'RT: Ambient Occlusion Pre-Trace',
        'urtx_rtaoscreenspace': 'RT: Ambient Occlusion Screen-Space Sampling',
        'urtx_rthbil': 'RT: Screen-Space Indirect Lighting (HBIL)',
        'urtx_rtreflfog': 'RT/PT: Fog in Ray-Traced Reflections',
        'urtx_rtreflscreenspace': 'RT: Screen-Space Reflection Assist',
        'urtx_terrainpreload': 'Terrain Preload Distance',
        'urtx_texturestreams': 'Textures Streamed at Once',
        'urtx_w_keep': '保留资源',
        'urtx_w_off': '关',
        'urtx_w_on': '开'},
 'cz': {'cr01_pt_scene_range': 'PT: Scene Range',
        'option_cr01_pt_scene_range': 'PT: Scene Range',
        'option_urt52_sep_cpu': ' CPU\xa0',
        'option_urt52_sep_fg': ' Frame Generation\xa0',
        'option_urt52_sep_pt': ' Path Tracing\xa0',
        'option_urt52_sep_rt': ' Ray Tracing\xa0',
        'option_urt52_sep_rtgip': ' RTGI: výkon\xa0',
        'option_urt52_sep_rtgiq': ' RTGI: kvalita\xa0',
        'option_urt52_sep_rtpt': ' RT/PT: společné\xa0',
        'option_urt52_sep_tex': ' Textury / vykreslování\xa0',
        'option_urtx_fgmenuresources': 'Frame Generation: Keep Resources in Menus',
        'option_urtx_ptaoquality': 'PT: Ambient Occlusion Quality',
        'option_urtx_ptskinbouncedesat': 'PT: Skin Bounce Light Desaturation',
        'option_urtx_rtaopretrace': 'RT: Ambient Occlusion Pre-Trace',
        'option_urtx_rtaoscreenspace': 'RT: Ambient Occlusion Screen-Space Sampling',
        'option_urtx_rthbil': 'RT: Screen-Space Indirect Lighting (HBIL)',
        'option_urtx_rtreflfog': 'RT/PT: Fog in Ray-Traced Reflections',
        'option_urtx_rtreflscreenspace': 'RT: Screen-Space Reflection Assist',
        'option_urtx_terrainpreload': 'Terrain Preload Distance',
        'option_urtx_texturestreams': 'Textures Streamed at Once',
        'preset_value_urt_m150': '150 m',
        'preset_value_urt_m200': '200 m',
        'preset_value_urt_m250': '250 m',
        'preset_value_urt_m300': '300 m',
        'tooltip_cr01_pt_scene_range': 'Zmenšuje oblast ray tracingu kolem vás (hra: 400 m); vzdálené odrazy, nepřímé '
                                       'světlo a stíny mohou slábnout. Nižší hodnoty mohou zvýšit FPS. Pouze Path '
                                       'Tracing. Projeví se okamžitě.',
        'tooltip_urtx_fgmenuresources': 'Ponechá prostředky Frame Generation, dokud je otevřené menu přes celou '
                                        'obrazovku, což může zabránit záseku po návratu. Žádné jiné nastavení Frame '
                                        'Generation, Reflex ani V-Sync se nemění. Při otevřeném menu využívá více '
                                        'videopaměti; FPS ve hře nemění. Projeví se okamžitě.',
        'tooltip_urtx_ptaoquality': 'Kvalita okluze okolí při path tracingu; nižší úrovně mají více šumu. Nižší '
                                    'hodnoty mohou zvýšit FPS. Pouze Path Tracing. Projeví se okamžitě.',
        'tooltip_urtx_ptskinbouncedesat': 'Odebírá barvu světlu odraženému na kůži; vyšší hodnoty dávají neutrálnější '
                                          'odstíny pleti. Vzhledová volba; bez prokázaného přínosu pro FPS. Pouze Path '
                                          'Tracing. Projeví se okamžitě.',
        'tooltip_urtx_rtaopretrace': 'Spustí průchod předběžného trasování okluze okolí s ray tracingem. Vliv na FPS '
                                     'závisí na scéně. Pouze běžný Ray Tracing. Projeví se okamžitě.',
        'tooltip_urtx_rtaoscreenspace': 'Nechá okolní zastínění z ray tracingu používat vzorky z obrazovky (výchozí '
                                        'Zap.). Vyp. používá jen paprsky. Vliv na FPS závisí na scéně. Pouze běžný Ray '
                                        'Tracing. Projeví se okamžitě.',
        'tooltip_urtx_rthbil': 'Přidá nepřímé světlo z prostoru obrazovky (HBIL) ke globálnímu osvětlení s ray '
                               'tracingem. Vypnuto může zvýšit FPS. Pouze běžný Ray Tracing. Projeví se okamžitě.',
        'tooltip_urtx_rtreflfog': 'Použije mlhu uvnitř odrazů z ray tracingu. Vyp. dá v mlze čistší odrazy a ušetří '
                                  'trochu času GPU. Vypnuto může zvýšit FPS. Běžný Ray Tracing a Path Tracing. Projeví '
                                  'se okamžitě.',
        'tooltip_urtx_rtreflscreenspace': 'Umožní odrazům použít data z prostoru obrazovky před trasováním paprsků. '
                                          'Zapnuto může zvýšit FPS. Pouze běžný Ray Tracing. Projeví se okamžitě.',
        'tooltip_urtx_terrainpreload': 'Jak daleko dopředu se terén načítá ve směru pohybu (nativně 20 m). Vyšší '
                                       'hodnoty omezují doskakování objektů, ale přidávají načítání. Projeví se v nově '
                                       'načtených oblastech.',
        'tooltip_urtx_texturestreams': 'Počet textur načítaných současně (nativně 12). Vyšší hodnoty omezují '
                                       'doskakování objektů, ale přidávají načítání. Projeví se okamžitě.',
        'urt52_sep_cpu': ' CPU\xa0',
        'urt52_sep_fg': ' Frame Generation\xa0',
        'urt52_sep_pt': ' Path Tracing\xa0',
        'urt52_sep_rt': ' Ray Tracing\xa0',
        'urt52_sep_rtgip': ' RTGI: výkon\xa0',
        'urt52_sep_rtgiq': ' RTGI: kvalita\xa0',
        'urt52_sep_rtpt': ' RT/PT: společné\xa0',
        'urt52_sep_tex': ' Textury / vykreslování\xa0',
        'urtx_fgmenuresources': 'Frame Generation: Keep Resources in Menus',
        'urtx_n_0': '0',
        'urtx_n_0p005': '0.005',
        'urtx_n_0p015': '0.015',
        'urtx_n_0p02': '0.02',
        'urtx_n_0p03': '0.03',
        'urtx_n_0p05': '0.05',
        'urtx_n_0p1': '0.1',
        'urtx_n_0p25': '0.25',
        'urtx_n_0p5': '0.5',
        'urtx_n_0p75': '0.75',
        'urtx_n_1': '1',
        'urtx_n_10000_mb': '10000 MB',
        'urtx_n_100_m': '100 m',
        'urtx_n_10_m': '10 m',
        'urtx_n_16': '16',
        'urtx_n_1600_mb': '1600 MB',
        'urtx_n_2': '2',
        'urtx_n_2000_mb': '2000 MB',
        'urtx_n_200_m': '200 m',
        'urtx_n_24': '24',
        'urtx_n_3': '3',
        'urtx_n_3000_mb': '3000 MB',
        'urtx_n_32': '32',
        'urtx_n_4000_mb': '4000 MB',
        'urtx_n_400_m': '400 m',
        'urtx_n_40_m': '40 m',
        'urtx_n_48': '48',
        'urtx_n_6000_mb': '6000 MB',
        'urtx_n_600_m': '600 m',
        'urtx_n_64': '64',
        'urtx_n_8': '8',
        'urtx_n_8000_mb': '8000 MB',
        'urtx_n_minus0p5': '-0.5',
        'urtx_n_minus1': '-1',
        'urtx_n_minus1p5': '-1.5',
        'urtx_n_minus2': '-2',
        'urtx_ptaoquality': 'PT: Ambient Occlusion Quality',
        'urtx_ptskinbouncedesat': 'PT: Skin Bounce Light Desaturation',
        'urtx_rtaopretrace': 'RT: Ambient Occlusion Pre-Trace',
        'urtx_rtaoscreenspace': 'RT: Ambient Occlusion Screen-Space Sampling',
        'urtx_rthbil': 'RT: Screen-Space Indirect Lighting (HBIL)',
        'urtx_rtreflfog': 'RT/PT: Fog in Ray-Traced Reflections',
        'urtx_rtreflscreenspace': 'RT: Screen-Space Reflection Assist',
        'urtx_terrainpreload': 'Terrain Preload Distance',
        'urtx_texturestreams': 'Textures Streamed at Once',
        'urtx_w_keep': 'Ponechat prostředky',
        'urtx_w_off': 'Vyp.',
        'urtx_w_on': 'Zap.'},
 'de': {'cr01_pt_scene_range': 'PT: Scene Range',
        'option_cr01_pt_scene_range': 'PT: Scene Range',
        'option_urt52_sep_cpu': ' CPU\xa0',
        'option_urt52_sep_fg': ' Frame Generation\xa0',
        'option_urt52_sep_pt': ' Path Tracing\xa0',
        'option_urt52_sep_rt': ' Ray Tracing\xa0',
        'option_urt52_sep_rtgip': ' RTGI: Leistung\xa0',
        'option_urt52_sep_rtgiq': ' RTGI: Qualität\xa0',
        'option_urt52_sep_rtpt': ' RT/PT: Gemeinsam\xa0',
        'option_urt52_sep_tex': ' Texturen / Rendering\xa0',
        'option_urtx_fgmenuresources': 'Frame Generation: Keep Resources in Menus',
        'option_urtx_ptaoquality': 'PT: Ambient Occlusion Quality',
        'option_urtx_ptskinbouncedesat': 'PT: Skin Bounce Light Desaturation',
        'option_urtx_rtaopretrace': 'RT: Ambient Occlusion Pre-Trace',
        'option_urtx_rtaoscreenspace': 'RT: Ambient Occlusion Screen-Space Sampling',
        'option_urtx_rthbil': 'RT: Screen-Space Indirect Lighting (HBIL)',
        'option_urtx_rtreflfog': 'RT/PT: Fog in Ray-Traced Reflections',
        'option_urtx_rtreflscreenspace': 'RT: Screen-Space Reflection Assist',
        'option_urtx_terrainpreload': 'Terrain Preload Distance',
        'option_urtx_texturestreams': 'Textures Streamed at Once',
        'preset_value_urt_m150': '150 m',
        'preset_value_urt_m200': '200 m',
        'preset_value_urt_m250': '250 m',
        'preset_value_urt_m300': '300 m',
        'tooltip_cr01_pt_scene_range': 'Verkleinert den raytracing-Bereich um dich herum (Spiel: 400 m); entfernte '
                                       'Spiegelungen, indirektes Licht und Schatten können abnehmen. Niedrigere Werte '
                                       'können die FPS erhöhen. Nur Path Tracing. Wirkt sofort.',
        'tooltip_urtx_fgmenuresources': 'Behält die Ressourcen der Frame Generation, solange ein Vollbildmenü offen '
                                        'ist; das kann einen Ruckler bei der Rückkehr vermeiden. Andere Einstellungen '
                                        'von Frame Generation, Reflex oder V-Sync bleiben unverändert. Belegt '
                                        'zusätzlichen Videospeicher, solange ein Menü offen ist; keine FPS-Änderung im '
                                        'Spiel. Wirkt sofort.',
        'tooltip_urtx_ptaoquality': 'Qualität der path-traced Umgebungsverdeckung; niedrigere Stufen wirken '
                                    'rauschiger. Niedrigere Werte können die FPS erhöhen. Nur Path Tracing. Wirkt '
                                    'sofort.',
        'tooltip_urtx_ptskinbouncedesat': 'Entfernt Farbe aus Licht, das auf Haut zurückgeworfen wird; höhere Werte '
                                          'ergeben neutralere Hauttöne. Optische Vorliebe; kein nachgewiesener '
                                          'FPS-Vorteil. Nur Path Tracing. Wirkt sofort.',
        'tooltip_urtx_rtaopretrace': 'Führt den Vorverfolgungsdurchgang der raytracing-Umgebungsverdeckung aus. Die '
                                     'Wirkung auf die FPS hängt von der Szene ab. Nur normales RT. Wirkt sofort.',
        'tooltip_urtx_rtaoscreenspace': 'Lässt die raytracing-Umgebungsverdeckung Bildschirm-Samples nutzen '
                                        '(Spielstandard An). Aus nutzt nur Strahlen. Die Wirkung auf die FPS hängt von '
                                        'der Szene ab. Nur normales RT. Wirkt sofort.',
        'tooltip_urtx_rthbil': 'Fügt indirektes Bildschirmraum-Licht (HBIL) zur raytracing-Globalbeleuchtung hinzu. '
                               'Aus kann die FPS erhöhen. Nur normales RT. Wirkt sofort.',
        'tooltip_urtx_rtreflfog': 'Wendet Nebel in raytracing-Spiegelungen an. Aus ergibt klarere Spiegelungen bei '
                                  'Nebel und spart etwas GPU-Zeit. Aus kann die FPS erhöhen. Normales RT und Path '
                                  'Tracing. Wirkt sofort.',
        'tooltip_urtx_rtreflscreenspace': 'Lässt Spiegelungen Bildschirmraum-Daten nutzen, bevor Strahlen verfolgt '
                                          'werden. An kann die FPS erhöhen. Nur normales RT. Wirkt sofort.',
        'tooltip_urtx_terrainpreload': 'Wie weit das Gelände in Bewegungsrichtung im Voraus geladen wird (nativ 20 m). '
                                       'Höhere Werte verringern Pop-in, erzeugen aber mehr Ladearbeit. Wirkt für neu '
                                       'geladene Gebiete.',
        'tooltip_urtx_texturestreams': 'Anzahl der gleichzeitig geladenen Texturen (nativ 12). Höhere Werte verringern '
                                       'Pop-in, erzeugen aber mehr Ladearbeit. Wirkt sofort.',
        'urt52_sep_cpu': ' CPU\xa0',
        'urt52_sep_fg': ' Frame Generation\xa0',
        'urt52_sep_pt': ' Path Tracing\xa0',
        'urt52_sep_rt': ' Ray Tracing\xa0',
        'urt52_sep_rtgip': ' RTGI: Leistung\xa0',
        'urt52_sep_rtgiq': ' RTGI: Qualität\xa0',
        'urt52_sep_rtpt': ' RT/PT: Gemeinsam\xa0',
        'urt52_sep_tex': ' Texturen / Rendering\xa0',
        'urtx_fgmenuresources': 'Frame Generation: Keep Resources in Menus',
        'urtx_n_0': '0',
        'urtx_n_0p005': '0.005',
        'urtx_n_0p015': '0.015',
        'urtx_n_0p02': '0.02',
        'urtx_n_0p03': '0.03',
        'urtx_n_0p05': '0.05',
        'urtx_n_0p1': '0.1',
        'urtx_n_0p25': '0.25',
        'urtx_n_0p5': '0.5',
        'urtx_n_0p75': '0.75',
        'urtx_n_1': '1',
        'urtx_n_10000_mb': '10000 MB',
        'urtx_n_100_m': '100 m',
        'urtx_n_10_m': '10 m',
        'urtx_n_16': '16',
        'urtx_n_1600_mb': '1600 MB',
        'urtx_n_2': '2',
        'urtx_n_2000_mb': '2000 MB',
        'urtx_n_200_m': '200 m',
        'urtx_n_24': '24',
        'urtx_n_3': '3',
        'urtx_n_3000_mb': '3000 MB',
        'urtx_n_32': '32',
        'urtx_n_4000_mb': '4000 MB',
        'urtx_n_400_m': '400 m',
        'urtx_n_40_m': '40 m',
        'urtx_n_48': '48',
        'urtx_n_6000_mb': '6000 MB',
        'urtx_n_600_m': '600 m',
        'urtx_n_64': '64',
        'urtx_n_8': '8',
        'urtx_n_8000_mb': '8000 MB',
        'urtx_n_minus0p5': '-0.5',
        'urtx_n_minus1': '-1',
        'urtx_n_minus1p5': '-1.5',
        'urtx_n_minus2': '-2',
        'urtx_ptaoquality': 'PT: Ambient Occlusion Quality',
        'urtx_ptskinbouncedesat': 'PT: Skin Bounce Light Desaturation',
        'urtx_rtaopretrace': 'RT: Ambient Occlusion Pre-Trace',
        'urtx_rtaoscreenspace': 'RT: Ambient Occlusion Screen-Space Sampling',
        'urtx_rthbil': 'RT: Screen-Space Indirect Lighting (HBIL)',
        'urtx_rtreflfog': 'RT/PT: Fog in Ray-Traced Reflections',
        'urtx_rtreflscreenspace': 'RT: Screen-Space Reflection Assist',
        'urtx_terrainpreload': 'Terrain Preload Distance',
        'urtx_texturestreams': 'Textures Streamed at Once',
        'urtx_w_keep': 'Ressourcen behalten',
        'urtx_w_off': 'Aus',
        'urtx_w_on': 'An'},
 'en': {'cr01_pt_scene_range': 'PT: Scene Range',
        'option_cr01_pt_scene_range': 'PT: Scene Range',
        'option_urt52_sep_cpu': ' CPU\xa0',
        'option_urt52_sep_fg': ' Frame Generation\xa0',
        'option_urt52_sep_pt': ' Path Tracing\xa0',
        'option_urt52_sep_rt': ' Ray Tracing\xa0',
        'option_urt52_sep_rtgip': ' RTGI: Performance\xa0',
        'option_urt52_sep_rtgiq': ' RTGI: Quality\xa0',
        'option_urt52_sep_rtpt': ' RT/PT: Shared\xa0',
        'option_urt52_sep_tex': ' Textures / Rendering\xa0',
        'option_urtx_fgmenuresources': 'Frame Generation: Keep Resources in Menus',
        'option_urtx_ptaoquality': 'PT: Ambient Occlusion Quality',
        'option_urtx_ptskinbouncedesat': 'PT: Skin Bounce Light Desaturation',
        'option_urtx_rtaopretrace': 'RT: Ambient Occlusion Pre-Trace',
        'option_urtx_rtaoscreenspace': 'RT: Ambient Occlusion Screen-Space Sampling',
        'option_urtx_rthbil': 'RT: Screen-Space Indirect Lighting (HBIL)',
        'option_urtx_rtreflfog': 'RT/PT: Fog in Ray-Traced Reflections',
        'option_urtx_rtreflscreenspace': 'RT: Screen-Space Reflection Assist',
        'option_urtx_terrainpreload': 'Terrain Preload Distance',
        'option_urtx_texturestreams': 'Textures Streamed at Once',
        'preset_value_urt_m150': '150 m',
        'preset_value_urt_m200': '200 m',
        'preset_value_urt_m250': '250 m',
        'preset_value_urt_m300': '300 m',
        'tooltip_cr01_pt_scene_range': 'Shrinks the ray-traced area around you (game: 400 m); distant reflections, '
                                       'bounced light and shadows may be reduced. Lower values may improve FPS. Path '
                                       'Tracing only. Applies immediately.',
        'tooltip_urtx_fgmenuresources': 'Keeps Frame Generation resources while a full-screen menu is open, which may '
                                        'avoid a hitch when you return. No other Frame Generation, Reflex or V-Sync '
                                        'setting is changed. Uses extra video memory while a menu is open; no FPS '
                                        'change during play. Applies immediately.',
        'tooltip_urtx_ptaoquality': 'Quality of the path-traced ambient occlusion pass; lower levels look noisier. '
                                    'Lower values may improve FPS. Path Tracing only. Applies immediately.',
        'tooltip_urtx_ptskinbouncedesat': 'Removes colour from light bouncing onto skin; higher values give more '
                                          'neutral skin tones. Visual preference; no established FPS benefit. Path '
                                          'Tracing only. Applies immediately.',
        'tooltip_urtx_rtaopretrace': 'Runs the pre-trace pass for ray-traced ambient occlusion. The FPS effect depends '
                                     'on the scene. Normal RT only. Applies immediately.',
        'tooltip_urtx_rtaoscreenspace': 'Lets ray-traced ambient occlusion use screen-space samples (game default On). '
                                        'Off uses rays only. The FPS effect depends on the scene. Normal RT only. '
                                        'Applies immediately.',
        'tooltip_urtx_rthbil': 'Adds screen-space bounce light (HBIL) on top of ray-traced global illumination. Off '
                               'may improve FPS. Normal RT only. Applies immediately.',
        'tooltip_urtx_rtreflfog': 'Applies fog inside ray-traced reflections. Off gives clearer reflections in foggy '
                                  'weather and saves a little GPU time. Off may improve FPS. Normal RT and Path '
                                  'Tracing. Applies immediately.',
        'tooltip_urtx_rtreflscreenspace': 'Lets reflections reuse screen-space data before tracing rays. On may '
                                          'improve FPS. Normal RT only. Applies immediately.',
        'tooltip_urtx_terrainpreload': 'How far ahead terrain is loaded in your direction of travel (native 20 m). '
                                       'Higher values reduce pop-in but add loading work. Applies to newly loaded '
                                       'areas.',
        'tooltip_urtx_texturestreams': 'Number of textures loaded at the same time (native 12). Higher values reduce '
                                       'pop-in but add loading work. Applies immediately.',
        'urt52_sep_cpu': ' CPU\xa0',
        'urt52_sep_fg': ' Frame Generation\xa0',
        'urt52_sep_pt': ' Path Tracing\xa0',
        'urt52_sep_rt': ' Ray Tracing\xa0',
        'urt52_sep_rtgip': ' RTGI: Performance\xa0',
        'urt52_sep_rtgiq': ' RTGI: Quality\xa0',
        'urt52_sep_rtpt': ' RT/PT: Shared\xa0',
        'urt52_sep_tex': ' Textures / Rendering\xa0',
        'urtx_fgmenuresources': 'Frame Generation: Keep Resources in Menus',
        'urtx_n_0': '0',
        'urtx_n_0p005': '0.005',
        'urtx_n_0p015': '0.015',
        'urtx_n_0p02': '0.02',
        'urtx_n_0p03': '0.03',
        'urtx_n_0p05': '0.05',
        'urtx_n_0p1': '0.1',
        'urtx_n_0p25': '0.25',
        'urtx_n_0p5': '0.5',
        'urtx_n_0p75': '0.75',
        'urtx_n_1': '1',
        'urtx_n_10000_mb': '10000 MB',
        'urtx_n_100_m': '100 m',
        'urtx_n_10_m': '10 m',
        'urtx_n_16': '16',
        'urtx_n_1600_mb': '1600 MB',
        'urtx_n_2': '2',
        'urtx_n_2000_mb': '2000 MB',
        'urtx_n_200_m': '200 m',
        'urtx_n_24': '24',
        'urtx_n_3': '3',
        'urtx_n_3000_mb': '3000 MB',
        'urtx_n_32': '32',
        'urtx_n_4000_mb': '4000 MB',
        'urtx_n_400_m': '400 m',
        'urtx_n_40_m': '40 m',
        'urtx_n_48': '48',
        'urtx_n_6000_mb': '6000 MB',
        'urtx_n_600_m': '600 m',
        'urtx_n_64': '64',
        'urtx_n_8': '8',
        'urtx_n_8000_mb': '8000 MB',
        'urtx_n_minus0p5': '-0.5',
        'urtx_n_minus1': '-1',
        'urtx_n_minus1p5': '-1.5',
        'urtx_n_minus2': '-2',
        'urtx_ptaoquality': 'PT: Ambient Occlusion Quality',
        'urtx_ptskinbouncedesat': 'PT: Skin Bounce Light Desaturation',
        'urtx_rtaopretrace': 'RT: Ambient Occlusion Pre-Trace',
        'urtx_rtaoscreenspace': 'RT: Ambient Occlusion Screen-Space Sampling',
        'urtx_rthbil': 'RT: Screen-Space Indirect Lighting (HBIL)',
        'urtx_rtreflfog': 'RT/PT: Fog in Ray-Traced Reflections',
        'urtx_rtreflscreenspace': 'RT: Screen-Space Reflection Assist',
        'urtx_terrainpreload': 'Terrain Preload Distance',
        'urtx_texturestreams': 'Textures Streamed at Once',
        'urtx_w_keep': 'Keep resources',
        'urtx_w_off': 'Off',
        'urtx_w_on': 'On'},
 'es': {'cr01_pt_scene_range': 'PT: Scene Range',
        'option_cr01_pt_scene_range': 'PT: Scene Range',
        'option_urt52_sep_cpu': ' CPU\xa0',
        'option_urt52_sep_fg': ' Frame Generation\xa0',
        'option_urt52_sep_pt': ' Path Tracing\xa0',
        'option_urt52_sep_rt': ' Ray Tracing\xa0',
        'option_urt52_sep_rtgip': ' RTGI: rendimiento\xa0',
        'option_urt52_sep_rtgiq': ' RTGI: calidad\xa0',
        'option_urt52_sep_rtpt': ' RT/PT: compartido\xa0',
        'option_urt52_sep_tex': ' Texturas / Renderizado\xa0',
        'option_urtx_fgmenuresources': 'Frame Generation: Keep Resources in Menus',
        'option_urtx_ptaoquality': 'PT: Ambient Occlusion Quality',
        'option_urtx_ptskinbouncedesat': 'PT: Skin Bounce Light Desaturation',
        'option_urtx_rtaopretrace': 'RT: Ambient Occlusion Pre-Trace',
        'option_urtx_rtaoscreenspace': 'RT: Ambient Occlusion Screen-Space Sampling',
        'option_urtx_rthbil': 'RT: Screen-Space Indirect Lighting (HBIL)',
        'option_urtx_rtreflfog': 'RT/PT: Fog in Ray-Traced Reflections',
        'option_urtx_rtreflscreenspace': 'RT: Screen-Space Reflection Assist',
        'option_urtx_terrainpreload': 'Terrain Preload Distance',
        'option_urtx_texturestreams': 'Textures Streamed at Once',
        'preset_value_urt_m150': '150 m',
        'preset_value_urt_m200': '200 m',
        'preset_value_urt_m250': '250 m',
        'preset_value_urt_m300': '300 m',
        'tooltip_cr01_pt_scene_range': 'Reduce la zona con ray tracing a tu alrededor (juego: 400 m); los reflejos, la '
                                       'luz indirecta y las sombras lejanas pueden disminuir. Valores más bajos pueden '
                                       'mejorar los FPS. Solo Path Tracing. Se aplica de inmediato.',
        'tooltip_urtx_fgmenuresources': 'Conserva los recursos de Frame Generation mientras hay un menú a pantalla '
                                        'completa abierto, lo que puede evitar un tirón al volver. No cambia ningún '
                                        'otro ajuste de Frame Generation, Reflex o V-Sync. Usa más memoria de vídeo '
                                        'con un menú abierto; no cambia los FPS durante el juego. Se aplica de '
                                        'inmediato.',
        'tooltip_urtx_ptaoquality': 'Calidad de la oclusión ambiental con path tracing; los niveles bajos tienen más '
                                    'ruido. Valores más bajos pueden mejorar los FPS. Solo Path Tracing. Se aplica de '
                                    'inmediato.',
        'tooltip_urtx_ptskinbouncedesat': 'Quita color a la luz que rebota sobre la piel; valores más altos dan tonos '
                                          'de piel más neutros. Preferencia visual; sin beneficio de FPS demostrado. '
                                          'Solo Path Tracing. Se aplica de inmediato.',
        'tooltip_urtx_rtaopretrace': 'Ejecuta la pasada de pretrazado de la oclusión ambiental con ray tracing. El '
                                     'efecto en los FPS depende de la escena. Solo Ray Tracing normal. Se aplica de '
                                     'inmediato.',
        'tooltip_urtx_rtaoscreenspace': 'Permite que la oclusión ambiental con trazado de rayos use muestras de '
                                        'pantalla (activado por defecto). Desactivado usa solo rayos. El efecto en los '
                                        'FPS depende de la escena. Solo Ray Tracing normal. Se aplica de inmediato.',
        'tooltip_urtx_rthbil': 'Añade luz indirecta en espacio de pantalla (HBIL) a la iluminación global con ray '
                               'tracing. Desactivado puede mejorar los FPS. Solo Ray Tracing normal. Se aplica de '
                               'inmediato.',
        'tooltip_urtx_rtreflfog': 'Aplica niebla dentro de los reflejos con trazado de rayos. Desactivado da reflejos '
                                  'más claros con niebla y ahorra algo de tiempo de GPU. Desactivado puede mejorar los '
                                  'FPS. Ray Tracing normal y Path Tracing. Se aplica de inmediato.',
        'tooltip_urtx_rtreflscreenspace': 'Permite a los reflejos usar datos del espacio de pantalla antes de trazar '
                                          'rayos. Activado puede mejorar los FPS. Solo Ray Tracing normal. Se aplica '
                                          'de inmediato.',
        'tooltip_urtx_terrainpreload': 'Hasta qué distancia se precarga el terreno en tu dirección de marcha (nativo '
                                       '20 m). Valores más altos reducen el pop-in pero añaden trabajo de carga. Se '
                                       'aplica a las zonas cargadas de nuevo.',
        'tooltip_urtx_texturestreams': 'Número de texturas que se cargan a la vez (nativo 12). Valores más altos '
                                       'reducen el pop-in pero añaden trabajo de carga. Se aplica de inmediato.',
        'urt52_sep_cpu': ' CPU\xa0',
        'urt52_sep_fg': ' Frame Generation\xa0',
        'urt52_sep_pt': ' Path Tracing\xa0',
        'urt52_sep_rt': ' Ray Tracing\xa0',
        'urt52_sep_rtgip': ' RTGI: rendimiento\xa0',
        'urt52_sep_rtgiq': ' RTGI: calidad\xa0',
        'urt52_sep_rtpt': ' RT/PT: compartido\xa0',
        'urt52_sep_tex': ' Texturas / Renderizado\xa0',
        'urtx_fgmenuresources': 'Frame Generation: Keep Resources in Menus',
        'urtx_n_0': '0',
        'urtx_n_0p005': '0.005',
        'urtx_n_0p015': '0.015',
        'urtx_n_0p02': '0.02',
        'urtx_n_0p03': '0.03',
        'urtx_n_0p05': '0.05',
        'urtx_n_0p1': '0.1',
        'urtx_n_0p25': '0.25',
        'urtx_n_0p5': '0.5',
        'urtx_n_0p75': '0.75',
        'urtx_n_1': '1',
        'urtx_n_10000_mb': '10000 MB',
        'urtx_n_100_m': '100 m',
        'urtx_n_10_m': '10 m',
        'urtx_n_16': '16',
        'urtx_n_1600_mb': '1600 MB',
        'urtx_n_2': '2',
        'urtx_n_2000_mb': '2000 MB',
        'urtx_n_200_m': '200 m',
        'urtx_n_24': '24',
        'urtx_n_3': '3',
        'urtx_n_3000_mb': '3000 MB',
        'urtx_n_32': '32',
        'urtx_n_4000_mb': '4000 MB',
        'urtx_n_400_m': '400 m',
        'urtx_n_40_m': '40 m',
        'urtx_n_48': '48',
        'urtx_n_6000_mb': '6000 MB',
        'urtx_n_600_m': '600 m',
        'urtx_n_64': '64',
        'urtx_n_8': '8',
        'urtx_n_8000_mb': '8000 MB',
        'urtx_n_minus0p5': '-0.5',
        'urtx_n_minus1': '-1',
        'urtx_n_minus1p5': '-1.5',
        'urtx_n_minus2': '-2',
        'urtx_ptaoquality': 'PT: Ambient Occlusion Quality',
        'urtx_ptskinbouncedesat': 'PT: Skin Bounce Light Desaturation',
        'urtx_rtaopretrace': 'RT: Ambient Occlusion Pre-Trace',
        'urtx_rtaoscreenspace': 'RT: Ambient Occlusion Screen-Space Sampling',
        'urtx_rthbil': 'RT: Screen-Space Indirect Lighting (HBIL)',
        'urtx_rtreflfog': 'RT/PT: Fog in Ray-Traced Reflections',
        'urtx_rtreflscreenspace': 'RT: Screen-Space Reflection Assist',
        'urtx_terrainpreload': 'Terrain Preload Distance',
        'urtx_texturestreams': 'Textures Streamed at Once',
        'urtx_w_keep': 'Conservar recursos',
        'urtx_w_off': 'Desactivado',
        'urtx_w_on': 'Activado'},
 'esmx': {'cr01_pt_scene_range': 'PT: Scene Range',
          'option_cr01_pt_scene_range': 'PT: Scene Range',
          'option_urt52_sep_cpu': ' CPU\xa0',
          'option_urt52_sep_fg': ' Frame Generation\xa0',
          'option_urt52_sep_pt': ' Path Tracing\xa0',
          'option_urt52_sep_rt': ' Ray Tracing\xa0',
          'option_urt52_sep_rtgip': ' RTGI: rendimiento\xa0',
          'option_urt52_sep_rtgiq': ' RTGI: calidad\xa0',
          'option_urt52_sep_rtpt': ' RT/PT: compartido\xa0',
          'option_urt52_sep_tex': ' Texturas / Renderizado\xa0',
          'option_urtx_fgmenuresources': 'Frame Generation: Keep Resources in Menus',
          'option_urtx_ptaoquality': 'PT: Ambient Occlusion Quality',
          'option_urtx_ptskinbouncedesat': 'PT: Skin Bounce Light Desaturation',
          'option_urtx_rtaopretrace': 'RT: Ambient Occlusion Pre-Trace',
          'option_urtx_rtaoscreenspace': 'RT: Ambient Occlusion Screen-Space Sampling',
          'option_urtx_rthbil': 'RT: Screen-Space Indirect Lighting (HBIL)',
          'option_urtx_rtreflfog': 'RT/PT: Fog in Ray-Traced Reflections',
          'option_urtx_rtreflscreenspace': 'RT: Screen-Space Reflection Assist',
          'option_urtx_terrainpreload': 'Terrain Preload Distance',
          'option_urtx_texturestreams': 'Textures Streamed at Once',
          'preset_value_urt_m150': '150 m',
          'preset_value_urt_m200': '200 m',
          'preset_value_urt_m250': '250 m',
          'preset_value_urt_m300': '300 m',
          'tooltip_cr01_pt_scene_range': 'Reduce la zona con ray tracing a tu alrededor (juego: 400 m); los reflejos, '
                                         'la luz indirecta y las sombras lejanas pueden disminuir. Valores más bajos '
                                         'pueden mejorar los FPS. Solo Path Tracing. Se aplica de inmediato.',
          'tooltip_urtx_fgmenuresources': 'Conserva los recursos de Frame Generation mientras hay un menú a pantalla '
                                          'completa abierto, lo que puede evitar un tirón al volver. No cambia ningún '
                                          'otro ajuste de Frame Generation, Reflex o V-Sync. Usa más memoria de vídeo '
                                          'con un menú abierto; no cambia los FPS durante el juego. Se aplica de '
                                          'inmediato.',
          'tooltip_urtx_ptaoquality': 'Calidad de la oclusión ambiental con path tracing; los niveles bajos tienen más '
                                      'ruido. Valores más bajos pueden mejorar los FPS. Solo Path Tracing. Se aplica '
                                      'de inmediato.',
          'tooltip_urtx_ptskinbouncedesat': 'Quita color a la luz que rebota sobre la piel; valores más altos dan '
                                            'tonos de piel más neutros. Preferencia visual; sin beneficio de FPS '
                                            'demostrado. Solo Path Tracing. Se aplica de inmediato.',
          'tooltip_urtx_rtaopretrace': 'Ejecuta la pasada de pretrazado de la oclusión ambiental con ray tracing. El '
                                       'efecto en los FPS depende de la escena. Solo Ray Tracing normal. Se aplica de '
                                       'inmediato.',
          'tooltip_urtx_rtaoscreenspace': 'Permite que la oclusión ambiental con trazado de rayos use muestras de '
                                          'pantalla (activado por defecto). Desactivado usa solo rayos. El efecto en '
                                          'los FPS depende de la escena. Solo Ray Tracing normal. Se aplica de '
                                          'inmediato.',
          'tooltip_urtx_rthbil': 'Añade luz indirecta en espacio de pantalla (HBIL) a la iluminación global con ray '
                                 'tracing. Desactivado puede mejorar los FPS. Solo Ray Tracing normal. Se aplica de '
                                 'inmediato.',
          'tooltip_urtx_rtreflfog': 'Aplica niebla dentro de los reflejos con trazado de rayos. Desactivado da '
                                    'reflejos más claros con niebla y ahorra algo de tiempo de GPU. Desactivado puede '
                                    'mejorar los FPS. Ray Tracing normal y Path Tracing. Se aplica de inmediato.',
          'tooltip_urtx_rtreflscreenspace': 'Permite a los reflejos usar datos del espacio de pantalla antes de trazar '
                                            'rayos. Activado puede mejorar los FPS. Solo Ray Tracing normal. Se aplica '
                                            'de inmediato.',
          'tooltip_urtx_terrainpreload': 'Hasta qué distancia se precarga el terreno en tu dirección de marcha (nativo '
                                         '20 m). Valores más altos reducen el pop-in pero añaden trabajo de carga. Se '
                                         'aplica a las zonas cargadas de nuevo.',
          'tooltip_urtx_texturestreams': 'Número de texturas que se cargan a la vez (nativo 12). Valores más altos '
                                         'reducen el pop-in pero añaden trabajo de carga. Se aplica de inmediato.',
          'urt52_sep_cpu': ' CPU\xa0',
          'urt52_sep_fg': ' Frame Generation\xa0',
          'urt52_sep_pt': ' Path Tracing\xa0',
          'urt52_sep_rt': ' Ray Tracing\xa0',
          'urt52_sep_rtgip': ' RTGI: rendimiento\xa0',
          'urt52_sep_rtgiq': ' RTGI: calidad\xa0',
          'urt52_sep_rtpt': ' RT/PT: compartido\xa0',
          'urt52_sep_tex': ' Texturas / Renderizado\xa0',
          'urtx_fgmenuresources': 'Frame Generation: Keep Resources in Menus',
          'urtx_n_0': '0',
          'urtx_n_0p005': '0.005',
          'urtx_n_0p015': '0.015',
          'urtx_n_0p02': '0.02',
          'urtx_n_0p03': '0.03',
          'urtx_n_0p05': '0.05',
          'urtx_n_0p1': '0.1',
          'urtx_n_0p25': '0.25',
          'urtx_n_0p5': '0.5',
          'urtx_n_0p75': '0.75',
          'urtx_n_1': '1',
          'urtx_n_10000_mb': '10000 MB',
          'urtx_n_100_m': '100 m',
          'urtx_n_10_m': '10 m',
          'urtx_n_16': '16',
          'urtx_n_1600_mb': '1600 MB',
          'urtx_n_2': '2',
          'urtx_n_2000_mb': '2000 MB',
          'urtx_n_200_m': '200 m',
          'urtx_n_24': '24',
          'urtx_n_3': '3',
          'urtx_n_3000_mb': '3000 MB',
          'urtx_n_32': '32',
          'urtx_n_4000_mb': '4000 MB',
          'urtx_n_400_m': '400 m',
          'urtx_n_40_m': '40 m',
          'urtx_n_48': '48',
          'urtx_n_6000_mb': '6000 MB',
          'urtx_n_600_m': '600 m',
          'urtx_n_64': '64',
          'urtx_n_8': '8',
          'urtx_n_8000_mb': '8000 MB',
          'urtx_n_minus0p5': '-0.5',
          'urtx_n_minus1': '-1',
          'urtx_n_minus1p5': '-1.5',
          'urtx_n_minus2': '-2',
          'urtx_ptaoquality': 'PT: Ambient Occlusion Quality',
          'urtx_ptskinbouncedesat': 'PT: Skin Bounce Light Desaturation',
          'urtx_rtaopretrace': 'RT: Ambient Occlusion Pre-Trace',
          'urtx_rtaoscreenspace': 'RT: Ambient Occlusion Screen-Space Sampling',
          'urtx_rthbil': 'RT: Screen-Space Indirect Lighting (HBIL)',
          'urtx_rtreflfog': 'RT/PT: Fog in Ray-Traced Reflections',
          'urtx_rtreflscreenspace': 'RT: Screen-Space Reflection Assist',
          'urtx_terrainpreload': 'Terrain Preload Distance',
          'urtx_texturestreams': 'Textures Streamed at Once',
          'urtx_w_keep': 'Conservar recursos',
          'urtx_w_off': 'Desactivado',
          'urtx_w_on': 'Activado'},
 'fr': {'cr01_pt_scene_range': 'PT: Scene Range',
        'option_cr01_pt_scene_range': 'PT: Scene Range',
        'option_urt52_sep_cpu': ' CPU\xa0',
        'option_urt52_sep_fg': ' Frame Generation\xa0',
        'option_urt52_sep_pt': ' Path Tracing\xa0',
        'option_urt52_sep_rt': ' Ray Tracing\xa0',
        'option_urt52_sep_rtgip': ' RTGI : performances\xa0',
        'option_urt52_sep_rtgiq': ' RTGI : qualité\xa0',
        'option_urt52_sep_rtpt': ' RT/PT : partagé\xa0',
        'option_urt52_sep_tex': ' Textures / rendu\xa0',
        'option_urtx_fgmenuresources': 'Frame Generation: Keep Resources in Menus',
        'option_urtx_ptaoquality': 'PT: Ambient Occlusion Quality',
        'option_urtx_ptskinbouncedesat': 'PT: Skin Bounce Light Desaturation',
        'option_urtx_rtaopretrace': 'RT: Ambient Occlusion Pre-Trace',
        'option_urtx_rtaoscreenspace': 'RT: Ambient Occlusion Screen-Space Sampling',
        'option_urtx_rthbil': 'RT: Screen-Space Indirect Lighting (HBIL)',
        'option_urtx_rtreflfog': 'RT/PT: Fog in Ray-Traced Reflections',
        'option_urtx_rtreflscreenspace': 'RT: Screen-Space Reflection Assist',
        'option_urtx_terrainpreload': 'Terrain Preload Distance',
        'option_urtx_texturestreams': 'Textures Streamed at Once',
        'preset_value_urt_m150': '150 m',
        'preset_value_urt_m200': '200 m',
        'preset_value_urt_m250': '250 m',
        'preset_value_urt_m300': '300 m',
        'tooltip_cr01_pt_scene_range': 'Réduit la zone en ray tracing autour de vous (jeu : 400 m) ; les réflexions, '
                                       'la lumière indirecte et les ombres lointaines peuvent diminuer. Des valeurs '
                                       "plus basses peuvent augmenter les FPS. Path Tracing uniquement. S'applique "
                                       'immédiatement.',
        'tooltip_urtx_fgmenuresources': "Conserve les ressources de Frame Generation pendant qu'un menu plein écran "
                                        'est ouvert, ce qui peut éviter une saccade au retour. Aucun autre réglage de '
                                        "Frame Generation, Reflex ou V-Sync n'est modifié. Utilise plus de mémoire "
                                        "vidéo quand un menu est ouvert ; aucun changement de FPS en jeu. S'applique "
                                        'immédiatement.',
        'tooltip_urtx_ptaoquality': "Qualité de la passe d'occlusion ambiante en path tracing ; les niveaux bas sont "
                                    'plus bruités. Des valeurs plus basses peuvent augmenter les FPS. Path Tracing '
                                    "uniquement. S'applique immédiatement.",
        'tooltip_urtx_ptskinbouncedesat': 'Retire la couleur de la lumière renvoyée sur la peau ; des valeurs plus '
                                          'hautes donnent des tons de peau plus neutres. Préférence visuelle ; aucun '
                                          "gain de FPS établi. Path Tracing uniquement. S'applique immédiatement.",
        'tooltip_urtx_rtaopretrace': "Exécute la passe de pré-traçage de l'occlusion ambiante en ray tracing. L'effet "
                                     "sur les FPS dépend de la scène. Ray Tracing normal uniquement. S'applique "
                                     'immédiatement.',
        'tooltip_urtx_rtaoscreenspace': "Permet à l'occlusion ambiante en ray tracing d'utiliser des échantillons de "
                                        "l'écran (activé par défaut). Désactivé n'utilise que des rayons. L'effet sur "
                                        "les FPS dépend de la scène. Ray Tracing normal uniquement. S'applique "
                                        'immédiatement.',
        'tooltip_urtx_rthbil': "Ajoute une lumière indirecte en espace écran (HBIL) à l'illumination globale en ray "
                               "tracing. Désactivé peut augmenter les FPS. Ray Tracing normal uniquement. S'applique "
                               'immédiatement.',
        'tooltip_urtx_rtreflfog': 'Applique le brouillard dans les reflets en ray tracing. Désactivé donne des reflets '
                                  'plus clairs par temps de brouillard et économise un peu de temps GPU. Désactivé '
                                  "peut augmenter les FPS. Ray Tracing normal et Path Tracing. S'applique "
                                  'immédiatement.',
        'tooltip_urtx_rtreflscreenspace': "Permet aux réflexions d'utiliser les données de l'espace écran avant de "
                                          'lancer des rayons. Activé peut augmenter les FPS. Ray Tracing normal '
                                          "uniquement. S'applique immédiatement.",
        'tooltip_urtx_terrainpreload': 'Distance à laquelle le terrain est préchargé dans votre direction de '
                                       'déplacement (natif 20 m). Des valeurs plus hautes réduisent le pop-in mais '
                                       "ajoutent du chargement. S'applique aux zones nouvellement chargées.",
        'tooltip_urtx_texturestreams': 'Nombre de textures chargées en même temps (natif 12). Des valeurs plus hautes '
                                       "réduisent le pop-in mais ajoutent du chargement. S'applique immédiatement.",
        'urt52_sep_cpu': ' CPU\xa0',
        'urt52_sep_fg': ' Frame Generation\xa0',
        'urt52_sep_pt': ' Path Tracing\xa0',
        'urt52_sep_rt': ' Ray Tracing\xa0',
        'urt52_sep_rtgip': ' RTGI : performances\xa0',
        'urt52_sep_rtgiq': ' RTGI : qualité\xa0',
        'urt52_sep_rtpt': ' RT/PT : partagé\xa0',
        'urt52_sep_tex': ' Textures / rendu\xa0',
        'urtx_fgmenuresources': 'Frame Generation: Keep Resources in Menus',
        'urtx_n_0': '0',
        'urtx_n_0p005': '0.005',
        'urtx_n_0p015': '0.015',
        'urtx_n_0p02': '0.02',
        'urtx_n_0p03': '0.03',
        'urtx_n_0p05': '0.05',
        'urtx_n_0p1': '0.1',
        'urtx_n_0p25': '0.25',
        'urtx_n_0p5': '0.5',
        'urtx_n_0p75': '0.75',
        'urtx_n_1': '1',
        'urtx_n_10000_mb': '10000 MB',
        'urtx_n_100_m': '100 m',
        'urtx_n_10_m': '10 m',
        'urtx_n_16': '16',
        'urtx_n_1600_mb': '1600 MB',
        'urtx_n_2': '2',
        'urtx_n_2000_mb': '2000 MB',
        'urtx_n_200_m': '200 m',
        'urtx_n_24': '24',
        'urtx_n_3': '3',
        'urtx_n_3000_mb': '3000 MB',
        'urtx_n_32': '32',
        'urtx_n_4000_mb': '4000 MB',
        'urtx_n_400_m': '400 m',
        'urtx_n_40_m': '40 m',
        'urtx_n_48': '48',
        'urtx_n_6000_mb': '6000 MB',
        'urtx_n_600_m': '600 m',
        'urtx_n_64': '64',
        'urtx_n_8': '8',
        'urtx_n_8000_mb': '8000 MB',
        'urtx_n_minus0p5': '-0.5',
        'urtx_n_minus1': '-1',
        'urtx_n_minus1p5': '-1.5',
        'urtx_n_minus2': '-2',
        'urtx_ptaoquality': 'PT: Ambient Occlusion Quality',
        'urtx_ptskinbouncedesat': 'PT: Skin Bounce Light Desaturation',
        'urtx_rtaopretrace': 'RT: Ambient Occlusion Pre-Trace',
        'urtx_rtaoscreenspace': 'RT: Ambient Occlusion Screen-Space Sampling',
        'urtx_rthbil': 'RT: Screen-Space Indirect Lighting (HBIL)',
        'urtx_rtreflfog': 'RT/PT: Fog in Ray-Traced Reflections',
        'urtx_rtreflscreenspace': 'RT: Screen-Space Reflection Assist',
        'urtx_terrainpreload': 'Terrain Preload Distance',
        'urtx_texturestreams': 'Textures Streamed at Once',
        'urtx_w_keep': 'Conserver les ressources',
        'urtx_w_off': 'Désactivé',
        'urtx_w_on': 'Activé'},
 'hu': {'cr01_pt_scene_range': 'PT: Scene Range',
        'option_cr01_pt_scene_range': 'PT: Scene Range',
        'option_urt52_sep_cpu': ' CPU\xa0',
        'option_urt52_sep_fg': ' Frame Generation\xa0',
        'option_urt52_sep_pt': ' Path Tracing\xa0',
        'option_urt52_sep_rt': ' Ray Tracing\xa0',
        'option_urt52_sep_rtgip': ' RTGI: teljesítmény\xa0',
        'option_urt52_sep_rtgiq': ' RTGI: minőség\xa0',
        'option_urt52_sep_rtpt': ' RT/PT: közös\xa0',
        'option_urt52_sep_tex': ' Textúrák / renderelés\xa0',
        'option_urtx_fgmenuresources': 'Frame Generation: Keep Resources in Menus',
        'option_urtx_ptaoquality': 'PT: Ambient Occlusion Quality',
        'option_urtx_ptskinbouncedesat': 'PT: Skin Bounce Light Desaturation',
        'option_urtx_rtaopretrace': 'RT: Ambient Occlusion Pre-Trace',
        'option_urtx_rtaoscreenspace': 'RT: Ambient Occlusion Screen-Space Sampling',
        'option_urtx_rthbil': 'RT: Screen-Space Indirect Lighting (HBIL)',
        'option_urtx_rtreflfog': 'RT/PT: Fog in Ray-Traced Reflections',
        'option_urtx_rtreflscreenspace': 'RT: Screen-Space Reflection Assist',
        'option_urtx_terrainpreload': 'Terrain Preload Distance',
        'option_urtx_texturestreams': 'Textures Streamed at Once',
        'preset_value_urt_m150': '150 m',
        'preset_value_urt_m200': '200 m',
        'preset_value_urt_m250': '250 m',
        'preset_value_urt_m300': '300 m',
        'tooltip_cr01_pt_scene_range': 'Csökkenti a körülötted lévő ray tracinges területet (játék: 400 m); a távoli '
                                       'tükröződések, szórt fény és árnyékok gyengülhetnek. Alacsonyabb értékek '
                                       'növelhetik az FPS-t. Csak Path Tracing. Azonnal érvényes.',
        'tooltip_urtx_fgmenuresources': 'Megtartja a Frame Generation erőforrásait, amíg teljes képernyős menü van '
                                        'nyitva, ami megelőzheti a visszatéréskor jelentkező akadást. A Frame '
                                        'Generation, a Reflex és a V-Sync többi beállítása nem változik. Nyitott menü '
                                        'alatt több videomemóriát használ; játék közben nem változtat az FPS-en. '
                                        'Azonnal érvényes.',
        'tooltip_urtx_ptaoquality': 'A path tracinges környezeti takarás minősége; alacsonyabb szintek zajosabbak. '
                                    'Alacsonyabb értékek növelhetik az FPS-t. Csak Path Tracing. Azonnal érvényes.',
        'tooltip_urtx_ptskinbouncedesat': 'Elveszi a színt a bőrre visszaverődő fényből; magasabb értékek semlegesebb '
                                          'bőrtónust adnak. Ízlés kérdése; igazolt FPS-előny nélkül. Csak Path '
                                          'Tracing. Azonnal érvényes.',
        'tooltip_urtx_rtaopretrace': 'Lefuttatja a ray tracinges környezeti takarás előkövetési menetét. Az FPS-re '
                                     'gyakorolt hatás a jelenettől függ. Csak normál Ray Tracing. Azonnal érvényes.',
        'tooltip_urtx_rtaoscreenspace': 'Engedi, hogy a sugárkövetett környezeti takarás képernyőmintákat használjon '
                                        '(alapból Be). Ki állásban csak sugarakat használ. Az FPS-re gyakorolt hatás a '
                                        'jelenettől függ. Csak normál Ray Tracing. Azonnal érvényes.',
        'tooltip_urtx_rthbil': 'Képtér-alapú szórt fényt (HBIL) ad a ray tracinges globális megvilágításhoz. A Ki '
                               'növelheti az FPS-t. Csak normál Ray Tracing. Azonnal érvényes.',
        'tooltip_urtx_rtreflfog': 'Ködöt alkalmaz a sugárkövetett tükröződésekben. Ki állásban ködben tisztábbak a '
                                  'tükröződések, és némi GPU-idő megtakarítható. A Ki növelheti az FPS-t. Normál Ray '
                                  'Tracing és Path Tracing. Azonnal érvényes.',
        'tooltip_urtx_rtreflscreenspace': 'Lehetővé teszi, hogy a tükröződések képtér-adatokat használjanak a sugarak '
                                          'követése előtt. A Be növelheti az FPS-t. Csak normál Ray Tracing. Azonnal '
                                          'érvényes.',
        'tooltip_urtx_terrainpreload': 'Milyen messzire töltődik be előre a terep a haladási irányodban (natív 20 m). '
                                       'Magasabb értékek csökkentik a beugrást, de több betöltéssel járnak. Az újonnan '
                                       'betöltött területeken érvényes.',
        'tooltip_urtx_texturestreams': 'Az egyszerre betöltött textúrák száma (natív 12). Magasabb értékek csökkentik '
                                       'a beugrást, de több betöltéssel járnak. Azonnal érvényes.',
        'urt52_sep_cpu': ' CPU\xa0',
        'urt52_sep_fg': ' Frame Generation\xa0',
        'urt52_sep_pt': ' Path Tracing\xa0',
        'urt52_sep_rt': ' Ray Tracing\xa0',
        'urt52_sep_rtgip': ' RTGI: teljesítmény\xa0',
        'urt52_sep_rtgiq': ' RTGI: minőség\xa0',
        'urt52_sep_rtpt': ' RT/PT: közös\xa0',
        'urt52_sep_tex': ' Textúrák / renderelés\xa0',
        'urtx_fgmenuresources': 'Frame Generation: Keep Resources in Menus',
        'urtx_n_0': '0',
        'urtx_n_0p005': '0.005',
        'urtx_n_0p015': '0.015',
        'urtx_n_0p02': '0.02',
        'urtx_n_0p03': '0.03',
        'urtx_n_0p05': '0.05',
        'urtx_n_0p1': '0.1',
        'urtx_n_0p25': '0.25',
        'urtx_n_0p5': '0.5',
        'urtx_n_0p75': '0.75',
        'urtx_n_1': '1',
        'urtx_n_10000_mb': '10000 MB',
        'urtx_n_100_m': '100 m',
        'urtx_n_10_m': '10 m',
        'urtx_n_16': '16',
        'urtx_n_1600_mb': '1600 MB',
        'urtx_n_2': '2',
        'urtx_n_2000_mb': '2000 MB',
        'urtx_n_200_m': '200 m',
        'urtx_n_24': '24',
        'urtx_n_3': '3',
        'urtx_n_3000_mb': '3000 MB',
        'urtx_n_32': '32',
        'urtx_n_4000_mb': '4000 MB',
        'urtx_n_400_m': '400 m',
        'urtx_n_40_m': '40 m',
        'urtx_n_48': '48',
        'urtx_n_6000_mb': '6000 MB',
        'urtx_n_600_m': '600 m',
        'urtx_n_64': '64',
        'urtx_n_8': '8',
        'urtx_n_8000_mb': '8000 MB',
        'urtx_n_minus0p5': '-0.5',
        'urtx_n_minus1': '-1',
        'urtx_n_minus1p5': '-1.5',
        'urtx_n_minus2': '-2',
        'urtx_ptaoquality': 'PT: Ambient Occlusion Quality',
        'urtx_ptskinbouncedesat': 'PT: Skin Bounce Light Desaturation',
        'urtx_rtaopretrace': 'RT: Ambient Occlusion Pre-Trace',
        'urtx_rtaoscreenspace': 'RT: Ambient Occlusion Screen-Space Sampling',
        'urtx_rthbil': 'RT: Screen-Space Indirect Lighting (HBIL)',
        'urtx_rtreflfog': 'RT/PT: Fog in Ray-Traced Reflections',
        'urtx_rtreflscreenspace': 'RT: Screen-Space Reflection Assist',
        'urtx_terrainpreload': 'Terrain Preload Distance',
        'urtx_texturestreams': 'Textures Streamed at Once',
        'urtx_w_keep': 'Erőforrások megtartása',
        'urtx_w_off': 'Ki',
        'urtx_w_on': 'Be'},
 'it': {'cr01_pt_scene_range': 'PT: Scene Range',
        'option_cr01_pt_scene_range': 'PT: Scene Range',
        'option_urt52_sep_cpu': ' CPU\xa0',
        'option_urt52_sep_fg': ' Frame Generation\xa0',
        'option_urt52_sep_pt': ' Path Tracing\xa0',
        'option_urt52_sep_rt': ' Ray Tracing\xa0',
        'option_urt52_sep_rtgip': ' RTGI: prestazioni\xa0',
        'option_urt52_sep_rtgiq': ' RTGI: qualità\xa0',
        'option_urt52_sep_rtpt': ' RT/PT: condiviso\xa0',
        'option_urt52_sep_tex': ' Texture / rendering\xa0',
        'option_urtx_fgmenuresources': 'Frame Generation: Keep Resources in Menus',
        'option_urtx_ptaoquality': 'PT: Ambient Occlusion Quality',
        'option_urtx_ptskinbouncedesat': 'PT: Skin Bounce Light Desaturation',
        'option_urtx_rtaopretrace': 'RT: Ambient Occlusion Pre-Trace',
        'option_urtx_rtaoscreenspace': 'RT: Ambient Occlusion Screen-Space Sampling',
        'option_urtx_rthbil': 'RT: Screen-Space Indirect Lighting (HBIL)',
        'option_urtx_rtreflfog': 'RT/PT: Fog in Ray-Traced Reflections',
        'option_urtx_rtreflscreenspace': 'RT: Screen-Space Reflection Assist',
        'option_urtx_terrainpreload': 'Terrain Preload Distance',
        'option_urtx_texturestreams': 'Textures Streamed at Once',
        'preset_value_urt_m150': '150 m',
        'preset_value_urt_m200': '200 m',
        'preset_value_urt_m250': '250 m',
        'preset_value_urt_m300': '300 m',
        'tooltip_cr01_pt_scene_range': "Riduce l'area in ray tracing intorno a te (gioco: 400 m); riflessi, luce "
                                       'indiretta e ombre lontane possono diminuire. Valori più bassi possono '
                                       'aumentare gli FPS. Solo Path Tracing. Si applica subito.',
        'tooltip_urtx_fgmenuresources': 'Mantiene le risorse di Frame Generation mentre è aperto un menu a schermo '
                                        "intero, il che può evitare uno scatto al ritorno. Nessun'altra impostazione "
                                        'di Frame Generation, Reflex o V-Sync viene cambiata. Usa più memoria video '
                                        'con un menu aperto; nessuna variazione di FPS durante il gioco. Si applica '
                                        'subito.',
        'tooltip_urtx_ptaoquality': "Qualità dell'occlusione ambientale in path tracing; i livelli bassi sono più "
                                    'rumorosi. Valori più bassi possono aumentare gli FPS. Solo Path Tracing. Si '
                                    'applica subito.',
        'tooltip_urtx_ptskinbouncedesat': 'Toglie colore alla luce che rimbalza sulla pelle; valori più alti danno '
                                          'toni della pelle più neutri. Preferenza visiva; nessun vantaggio di FPS '
                                          'accertato. Solo Path Tracing. Si applica subito.',
        'tooltip_urtx_rtaopretrace': "Esegue il passaggio di pre-tracciamento dell'occlusione ambientale in ray "
                                     "tracing. L'effetto sugli FPS dipende dalla scena. Solo Ray Tracing normale. Si "
                                     'applica subito.',
        'tooltip_urtx_rtaoscreenspace': "Permette all'occlusione ambientale in ray tracing di usare campioni dello "
                                        'schermo (attivo per impostazione predefinita). Disattivo usa solo raggi. '
                                        "L'effetto sugli FPS dipende dalla scena. Solo Ray Tracing normale. Si applica "
                                        'subito.',
        'tooltip_urtx_rthbil': "Aggiunge luce indiretta nello spazio schermo (HBIL) all'illuminazione globale in ray "
                               'tracing. Disattivato può aumentare gli FPS. Solo Ray Tracing normale. Si applica '
                               'subito.',
        'tooltip_urtx_rtreflfog': 'Applica la nebbia nei riflessi in ray tracing. Disattivo dà riflessi più nitidi con '
                                  "la nebbia e risparmia un po' di tempo di GPU. Disattivato può aumentare gli FPS. "
                                  'Ray Tracing normale e Path Tracing. Si applica subito.',
        'tooltip_urtx_rtreflscreenspace': 'Permette ai riflessi di usare i dati dello spazio schermo prima di '
                                          'tracciare i raggi. Attivato può aumentare gli FPS. Solo Ray Tracing '
                                          'normale. Si applica subito.',
        'tooltip_urtx_terrainpreload': 'Quanto lontano viene precaricato il terreno nella tua direzione di marcia '
                                       '(nativo 20 m). Valori più alti riducono il pop-in ma aggiungono caricamento. '
                                       'Si applica alle aree caricate di nuovo.',
        'tooltip_urtx_texturestreams': 'Numero di texture caricate contemporaneamente (nativo 12). Valori più alti '
                                       'riducono il pop-in ma aggiungono caricamento. Si applica subito.',
        'urt52_sep_cpu': ' CPU\xa0',
        'urt52_sep_fg': ' Frame Generation\xa0',
        'urt52_sep_pt': ' Path Tracing\xa0',
        'urt52_sep_rt': ' Ray Tracing\xa0',
        'urt52_sep_rtgip': ' RTGI: prestazioni\xa0',
        'urt52_sep_rtgiq': ' RTGI: qualità\xa0',
        'urt52_sep_rtpt': ' RT/PT: condiviso\xa0',
        'urt52_sep_tex': ' Texture / rendering\xa0',
        'urtx_fgmenuresources': 'Frame Generation: Keep Resources in Menus',
        'urtx_n_0': '0',
        'urtx_n_0p005': '0.005',
        'urtx_n_0p015': '0.015',
        'urtx_n_0p02': '0.02',
        'urtx_n_0p03': '0.03',
        'urtx_n_0p05': '0.05',
        'urtx_n_0p1': '0.1',
        'urtx_n_0p25': '0.25',
        'urtx_n_0p5': '0.5',
        'urtx_n_0p75': '0.75',
        'urtx_n_1': '1',
        'urtx_n_10000_mb': '10000 MB',
        'urtx_n_100_m': '100 m',
        'urtx_n_10_m': '10 m',
        'urtx_n_16': '16',
        'urtx_n_1600_mb': '1600 MB',
        'urtx_n_2': '2',
        'urtx_n_2000_mb': '2000 MB',
        'urtx_n_200_m': '200 m',
        'urtx_n_24': '24',
        'urtx_n_3': '3',
        'urtx_n_3000_mb': '3000 MB',
        'urtx_n_32': '32',
        'urtx_n_4000_mb': '4000 MB',
        'urtx_n_400_m': '400 m',
        'urtx_n_40_m': '40 m',
        'urtx_n_48': '48',
        'urtx_n_6000_mb': '6000 MB',
        'urtx_n_600_m': '600 m',
        'urtx_n_64': '64',
        'urtx_n_8': '8',
        'urtx_n_8000_mb': '8000 MB',
        'urtx_n_minus0p5': '-0.5',
        'urtx_n_minus1': '-1',
        'urtx_n_minus1p5': '-1.5',
        'urtx_n_minus2': '-2',
        'urtx_ptaoquality': 'PT: Ambient Occlusion Quality',
        'urtx_ptskinbouncedesat': 'PT: Skin Bounce Light Desaturation',
        'urtx_rtaopretrace': 'RT: Ambient Occlusion Pre-Trace',
        'urtx_rtaoscreenspace': 'RT: Ambient Occlusion Screen-Space Sampling',
        'urtx_rthbil': 'RT: Screen-Space Indirect Lighting (HBIL)',
        'urtx_rtreflfog': 'RT/PT: Fog in Ray-Traced Reflections',
        'urtx_rtreflscreenspace': 'RT: Screen-Space Reflection Assist',
        'urtx_terrainpreload': 'Terrain Preload Distance',
        'urtx_texturestreams': 'Textures Streamed at Once',
        'urtx_w_keep': 'Mantieni risorse',
        'urtx_w_off': 'Disattivo',
        'urtx_w_on': 'Attivo'},
 'jp': {'cr01_pt_scene_range': 'PT: Scene Range',
        'option_cr01_pt_scene_range': 'PT: Scene Range',
        'option_urt52_sep_cpu': ' CPU\xa0',
        'option_urt52_sep_fg': ' Frame Generation\xa0',
        'option_urt52_sep_pt': ' Path Tracing\xa0',
        'option_urt52_sep_rt': ' Ray Tracing\xa0',
        'option_urt52_sep_rtgip': ' RTGI: パフォーマンス\xa0',
        'option_urt52_sep_rtgiq': ' RTGI: 品質\xa0',
        'option_urt52_sep_rtpt': ' RT/PT: 共通\xa0',
        'option_urt52_sep_tex': ' テクスチャ / レンダリング\xa0',
        'option_urtx_fgmenuresources': 'Frame Generation: Keep Resources in Menus',
        'option_urtx_ptaoquality': 'PT: Ambient Occlusion Quality',
        'option_urtx_ptskinbouncedesat': 'PT: Skin Bounce Light Desaturation',
        'option_urtx_rtaopretrace': 'RT: Ambient Occlusion Pre-Trace',
        'option_urtx_rtaoscreenspace': 'RT: Ambient Occlusion Screen-Space Sampling',
        'option_urtx_rthbil': 'RT: Screen-Space Indirect Lighting (HBIL)',
        'option_urtx_rtreflfog': 'RT/PT: Fog in Ray-Traced Reflections',
        'option_urtx_rtreflscreenspace': 'RT: Screen-Space Reflection Assist',
        'option_urtx_terrainpreload': 'Terrain Preload Distance',
        'option_urtx_texturestreams': 'Textures Streamed at Once',
        'preset_value_urt_m150': '150 m',
        'preset_value_urt_m200': '200 m',
        'preset_value_urt_m250': '250 m',
        'preset_value_urt_m300': '300 m',
        'tooltip_cr01_pt_scene_range': '周囲のレイトレース範囲を狭めます（ゲーム既定: 400 m）。遠くの反射、間接光、影が弱まる場合があります。値を下げると FPS '
                                       'が向上する場合があります。Path Tracing のみ。すぐに反映されます。',
        'tooltip_urtx_fgmenuresources': 'フルスクリーンのメニューを開いている間 Frame Generation のリソースを保持し、戻ったときのカクつきを防げる場合があります。Frame '
                                        'Generation、Reflex、V-Sync のその他の設定は変わりません。メニューを開いている間ビデオメモリを多く使います。プレイ中の FPS '
                                        'は変わりません。すぐに反映されます。',
        'tooltip_urtx_ptaoquality': 'パストレースのアンビエントオクルージョンの品質です。低いレベルはノイズが増えます。値を下げると FPS が向上する場合があります。Path Tracing '
                                    'のみ。すぐに反映されます。',
        'tooltip_urtx_ptskinbouncedesat': '肌に跳ね返る光から色を取り除きます。値を上げると肌の色合いがより自然になります。見た目の好みの設定で、FPS 向上は確認されていません。Path '
                                          'Tracing のみ。すぐに反映されます。',
        'tooltip_urtx_rtaopretrace': 'レイトレースのアンビエントオクルージョンのプリトレースを実行します。FPS への影響はシーンによって異なります。通常の Ray Tracing '
                                     'のみ。すぐに反映されます。',
        'tooltip_urtx_rtaoscreenspace': 'レイトレースのアンビエントオクルージョンが画面サンプルを使えるようにします（既定はオン）。オフではレイのみを使います。FPS '
                                        'への影響はシーンによって異なります。通常の Ray Tracing のみ。すぐに反映されます。',
        'tooltip_urtx_rthbil': 'レイトレースのグローバルイルミネーションにスクリーンスペースの間接光（HBIL）を加えます。オフで FPS が向上する場合があります。通常の Ray Tracing '
                               'のみ。すぐに反映されます。',
        'tooltip_urtx_rtreflfog': 'レイトレース反射の中に霧を適用します。オフでは霧の中でも反射がくっきりし、GPU 時間を少し節約します。オフで FPS が向上する場合があります。通常の Ray '
                                  'Tracing と Path Tracing。すぐに反映されます。',
        'tooltip_urtx_rtreflscreenspace': 'レイをトレースする前に反射でスクリーンスペースのデータを利用します。オンで FPS が向上する場合があります。通常の Ray Tracing '
                                          'のみ。すぐに反映されます。',
        'tooltip_urtx_terrainpreload': '進行方向の地形をどれだけ先まで読み込むかです（ネイティブ 20 '
                                       'm）。値を上げるとポップインが減りますが、読み込み処理が増えます。新しく読み込まれたエリアに反映されます。',
        'tooltip_urtx_texturestreams': '同時に読み込むテクスチャの数です（ネイティブ 12）。値を上げるとポップインが減りますが、読み込み処理が増えます。すぐに反映されます。',
        'urt52_sep_cpu': ' CPU\xa0',
        'urt52_sep_fg': ' Frame Generation\xa0',
        'urt52_sep_pt': ' Path Tracing\xa0',
        'urt52_sep_rt': ' Ray Tracing\xa0',
        'urt52_sep_rtgip': ' RTGI: パフォーマンス\xa0',
        'urt52_sep_rtgiq': ' RTGI: 品質\xa0',
        'urt52_sep_rtpt': ' RT/PT: 共通\xa0',
        'urt52_sep_tex': ' テクスチャ / レンダリング\xa0',
        'urtx_fgmenuresources': 'Frame Generation: Keep Resources in Menus',
        'urtx_n_0': '0',
        'urtx_n_0p005': '0.005',
        'urtx_n_0p015': '0.015',
        'urtx_n_0p02': '0.02',
        'urtx_n_0p03': '0.03',
        'urtx_n_0p05': '0.05',
        'urtx_n_0p1': '0.1',
        'urtx_n_0p25': '0.25',
        'urtx_n_0p5': '0.5',
        'urtx_n_0p75': '0.75',
        'urtx_n_1': '1',
        'urtx_n_10000_mb': '10000 MB',
        'urtx_n_100_m': '100 m',
        'urtx_n_10_m': '10 m',
        'urtx_n_16': '16',
        'urtx_n_1600_mb': '1600 MB',
        'urtx_n_2': '2',
        'urtx_n_2000_mb': '2000 MB',
        'urtx_n_200_m': '200 m',
        'urtx_n_24': '24',
        'urtx_n_3': '3',
        'urtx_n_3000_mb': '3000 MB',
        'urtx_n_32': '32',
        'urtx_n_4000_mb': '4000 MB',
        'urtx_n_400_m': '400 m',
        'urtx_n_40_m': '40 m',
        'urtx_n_48': '48',
        'urtx_n_6000_mb': '6000 MB',
        'urtx_n_600_m': '600 m',
        'urtx_n_64': '64',
        'urtx_n_8': '8',
        'urtx_n_8000_mb': '8000 MB',
        'urtx_n_minus0p5': '-0.5',
        'urtx_n_minus1': '-1',
        'urtx_n_minus1p5': '-1.5',
        'urtx_n_minus2': '-2',
        'urtx_ptaoquality': 'PT: Ambient Occlusion Quality',
        'urtx_ptskinbouncedesat': 'PT: Skin Bounce Light Desaturation',
        'urtx_rtaopretrace': 'RT: Ambient Occlusion Pre-Trace',
        'urtx_rtaoscreenspace': 'RT: Ambient Occlusion Screen-Space Sampling',
        'urtx_rthbil': 'RT: Screen-Space Indirect Lighting (HBIL)',
        'urtx_rtreflfog': 'RT/PT: Fog in Ray-Traced Reflections',
        'urtx_rtreflscreenspace': 'RT: Screen-Space Reflection Assist',
        'urtx_terrainpreload': 'Terrain Preload Distance',
        'urtx_texturestreams': 'Textures Streamed at Once',
        'urtx_w_keep': 'リソースを保持',
        'urtx_w_off': 'オフ',
        'urtx_w_on': 'オン'},
 'kr': {'cr01_pt_scene_range': 'PT: Scene Range',
        'option_cr01_pt_scene_range': 'PT: Scene Range',
        'option_urt52_sep_cpu': ' CPU\xa0',
        'option_urt52_sep_fg': ' Frame Generation\xa0',
        'option_urt52_sep_pt': ' Path Tracing\xa0',
        'option_urt52_sep_rt': ' Ray Tracing\xa0',
        'option_urt52_sep_rtgip': ' RTGI: 성능\xa0',
        'option_urt52_sep_rtgiq': ' RTGI: 품질\xa0',
        'option_urt52_sep_rtpt': ' RT/PT: 공통\xa0',
        'option_urt52_sep_tex': ' 텍스처 / 렌더링\xa0',
        'option_urtx_fgmenuresources': 'Frame Generation: Keep Resources in Menus',
        'option_urtx_ptaoquality': 'PT: Ambient Occlusion Quality',
        'option_urtx_ptskinbouncedesat': 'PT: Skin Bounce Light Desaturation',
        'option_urtx_rtaopretrace': 'RT: Ambient Occlusion Pre-Trace',
        'option_urtx_rtaoscreenspace': 'RT: Ambient Occlusion Screen-Space Sampling',
        'option_urtx_rthbil': 'RT: Screen-Space Indirect Lighting (HBIL)',
        'option_urtx_rtreflfog': 'RT/PT: Fog in Ray-Traced Reflections',
        'option_urtx_rtreflscreenspace': 'RT: Screen-Space Reflection Assist',
        'option_urtx_terrainpreload': 'Terrain Preload Distance',
        'option_urtx_texturestreams': 'Textures Streamed at Once',
        'preset_value_urt_m150': '150 m',
        'preset_value_urt_m200': '200 m',
        'preset_value_urt_m250': '250 m',
        'preset_value_urt_m300': '300 m',
        'tooltip_cr01_pt_scene_range': '주변의 레이 트레이싱 범위를 줄입니다(게임 기본: 400 m). 먼 반사, 간접광, 그림자가 줄어들 수 있습니다. 값을 낮추면 FPS가 오를 '
                                       '수 있습니다. Path Tracing 전용입니다. 즉시 적용됩니다.',
        'tooltip_urtx_fgmenuresources': '전체 화면 메뉴가 열려 있는 동안 Frame Generation 리소스를 유지해, 돌아올 때의 끊김을 피할 수 있습니다. Frame '
                                        'Generation, Reflex, V-Sync의 다른 설정은 바뀌지 않습니다. 메뉴가 열려 있는 동안 비디오 메모리를 더 사용하며, '
                                        '플레이 중 FPS는 바뀌지 않습니다. 즉시 적용됩니다.',
        'tooltip_urtx_ptaoquality': '패스 트레이싱 앰비언트 오클루전의 품질입니다. 낮은 레벨은 노이즈가 많습니다. 값을 낮추면 FPS가 오를 수 있습니다. Path Tracing '
                                    '전용입니다. 즉시 적용됩니다.',
        'tooltip_urtx_ptskinbouncedesat': '피부에 반사되는 빛에서 색을 뺍니다. 값을 높이면 피부 톤이 더 중립적입니다. 외형 취향 설정이며 확인된 FPS 이점은 없습니다. '
                                          'Path Tracing 전용입니다. 즉시 적용됩니다.',
        'tooltip_urtx_rtaopretrace': '레이 트레이싱 앰비언트 오클루전의 사전 추적 패스를 실행합니다. FPS에 대한 영향은 장면에 따라 다릅니다. 일반 Ray Tracing '
                                     '전용입니다. 즉시 적용됩니다.',
        'tooltip_urtx_rtaoscreenspace': '레이 트레이싱 앰비언트 오클루전이 화면 샘플을 쓰게 합니다(기본 켜기). 끄기는 광선만 사용합니다. FPS에 대한 영향은 장면에 따라 '
                                        '다릅니다. 일반 Ray Tracing 전용입니다. 즉시 적용됩니다.',
        'tooltip_urtx_rthbil': '레이 트레이싱 전역 조명에 화면 공간 간접광(HBIL)을 더합니다. 끄기로 FPS가 오를 수 있습니다. 일반 Ray Tracing 전용입니다. 즉시 '
                               '적용됩니다.',
        'tooltip_urtx_rtreflfog': '레이 트레이싱 반사 안에 안개를 적용합니다. 끄기는 안개 속에서도 반사가 선명해지고 GPU 시간을 조금 아낍니다. 끄기로 FPS가 오를 수 있습니다. '
                                  '일반 Ray Tracing과 Path Tracing에 적용됩니다. 즉시 적용됩니다.',
        'tooltip_urtx_rtreflscreenspace': '광선을 추적하기 전에 반사가 화면 공간 데이터를 활용하게 합니다. 켜기로 FPS가 오를 수 있습니다. 일반 Ray Tracing '
                                          '전용입니다. 즉시 적용됩니다.',
        'tooltip_urtx_terrainpreload': '이동 방향으로 지형을 얼마나 앞서 불러오는지입니다(기본 20 m). 값을 높이면 팝인이 줄지만 로딩 작업이 늘어납니다. 새로 불러온 지역에 '
                                       '적용됩니다.',
        'tooltip_urtx_texturestreams': '동시에 불러오는 텍스처 수입니다(기본 12). 값을 높이면 팝인이 줄지만 로딩 작업이 늘어납니다. 즉시 적용됩니다.',
        'urt52_sep_cpu': ' CPU\xa0',
        'urt52_sep_fg': ' Frame Generation\xa0',
        'urt52_sep_pt': ' Path Tracing\xa0',
        'urt52_sep_rt': ' Ray Tracing\xa0',
        'urt52_sep_rtgip': ' RTGI: 성능\xa0',
        'urt52_sep_rtgiq': ' RTGI: 품질\xa0',
        'urt52_sep_rtpt': ' RT/PT: 공통\xa0',
        'urt52_sep_tex': ' 텍스처 / 렌더링\xa0',
        'urtx_fgmenuresources': 'Frame Generation: Keep Resources in Menus',
        'urtx_n_0': '0',
        'urtx_n_0p005': '0.005',
        'urtx_n_0p015': '0.015',
        'urtx_n_0p02': '0.02',
        'urtx_n_0p03': '0.03',
        'urtx_n_0p05': '0.05',
        'urtx_n_0p1': '0.1',
        'urtx_n_0p25': '0.25',
        'urtx_n_0p5': '0.5',
        'urtx_n_0p75': '0.75',
        'urtx_n_1': '1',
        'urtx_n_10000_mb': '10000 MB',
        'urtx_n_100_m': '100 m',
        'urtx_n_10_m': '10 m',
        'urtx_n_16': '16',
        'urtx_n_1600_mb': '1600 MB',
        'urtx_n_2': '2',
        'urtx_n_2000_mb': '2000 MB',
        'urtx_n_200_m': '200 m',
        'urtx_n_24': '24',
        'urtx_n_3': '3',
        'urtx_n_3000_mb': '3000 MB',
        'urtx_n_32': '32',
        'urtx_n_4000_mb': '4000 MB',
        'urtx_n_400_m': '400 m',
        'urtx_n_40_m': '40 m',
        'urtx_n_48': '48',
        'urtx_n_6000_mb': '6000 MB',
        'urtx_n_600_m': '600 m',
        'urtx_n_64': '64',
        'urtx_n_8': '8',
        'urtx_n_8000_mb': '8000 MB',
        'urtx_n_minus0p5': '-0.5',
        'urtx_n_minus1': '-1',
        'urtx_n_minus1p5': '-1.5',
        'urtx_n_minus2': '-2',
        'urtx_ptaoquality': 'PT: Ambient Occlusion Quality',
        'urtx_ptskinbouncedesat': 'PT: Skin Bounce Light Desaturation',
        'urtx_rtaopretrace': 'RT: Ambient Occlusion Pre-Trace',
        'urtx_rtaoscreenspace': 'RT: Ambient Occlusion Screen-Space Sampling',
        'urtx_rthbil': 'RT: Screen-Space Indirect Lighting (HBIL)',
        'urtx_rtreflfog': 'RT/PT: Fog in Ray-Traced Reflections',
        'urtx_rtreflscreenspace': 'RT: Screen-Space Reflection Assist',
        'urtx_terrainpreload': 'Terrain Preload Distance',
        'urtx_texturestreams': 'Textures Streamed at Once',
        'urtx_w_keep': '리소스 유지',
        'urtx_w_off': '끄기',
        'urtx_w_on': '켜기'},
 'pl': {'cr01_pt_scene_range': 'PT: Scene Range',
        'option_cr01_pt_scene_range': 'PT: Scene Range',
        'option_urt52_sep_cpu': ' CPU\xa0',
        'option_urt52_sep_fg': ' Frame Generation\xa0',
        'option_urt52_sep_pt': ' Path Tracing\xa0',
        'option_urt52_sep_rt': ' Ray Tracing\xa0',
        'option_urt52_sep_rtgip': ' RTGI: wydajność\xa0',
        'option_urt52_sep_rtgiq': ' RTGI: jakość\xa0',
        'option_urt52_sep_rtpt': ' RT/PT: wspólne\xa0',
        'option_urt52_sep_tex': ' Tekstury / renderowanie\xa0',
        'option_urtx_fgmenuresources': 'Frame Generation: Keep Resources in Menus',
        'option_urtx_ptaoquality': 'PT: Ambient Occlusion Quality',
        'option_urtx_ptskinbouncedesat': 'PT: Skin Bounce Light Desaturation',
        'option_urtx_rtaopretrace': 'RT: Ambient Occlusion Pre-Trace',
        'option_urtx_rtaoscreenspace': 'RT: Ambient Occlusion Screen-Space Sampling',
        'option_urtx_rthbil': 'RT: Screen-Space Indirect Lighting (HBIL)',
        'option_urtx_rtreflfog': 'RT/PT: Fog in Ray-Traced Reflections',
        'option_urtx_rtreflscreenspace': 'RT: Screen-Space Reflection Assist',
        'option_urtx_terrainpreload': 'Terrain Preload Distance',
        'option_urtx_texturestreams': 'Textures Streamed at Once',
        'preset_value_urt_m150': '150 m',
        'preset_value_urt_m200': '200 m',
        'preset_value_urt_m250': '250 m',
        'preset_value_urt_m300': '300 m',
        'tooltip_cr01_pt_scene_range': 'Zmniejsza obszar ray tracingu wokół ciebie (gra: 400 m); odległe odbicia, '
                                       'światło pośrednie i cienie mogą osłabnąć. Niższe wartości mogą zwiększyć FPS. '
                                       'Tylko Path Tracing. Działa od razu.',
        'tooltip_urtx_fgmenuresources': 'Zachowuje zasoby Frame Generation, gdy otwarte jest menu pełnoekranowe, co '
                                        'może zapobiec przycięciu po powrocie. Inne ustawienia Frame Generation, '
                                        'Reflex i V-Sync się nie zmieniają. Zużywa więcej pamięci wideo przy otwartym '
                                        'menu; nie zmienia FPS w grze. Działa od razu.',
        'tooltip_urtx_ptaoquality': 'Jakość okluzji otoczenia w path tracingu; niższe poziomy mają więcej szumu. '
                                    'Niższe wartości mogą zwiększyć FPS. Tylko Path Tracing. Działa od razu.',
        'tooltip_urtx_ptskinbouncedesat': 'Usuwa kolor ze światła odbitego na skórę; wyższe wartości dają bardziej '
                                          'neutralne odcienie skóry. Kwestia wyglądu; brak potwierdzonego zysku FPS. '
                                          'Tylko Path Tracing. Działa od razu.',
        'tooltip_urtx_rtaopretrace': 'Uruchamia przebieg wstępnego śledzenia okluzji otoczenia z ray tracingiem. Wpływ '
                                     'na FPS zależy od sceny. Tylko zwykły Ray Tracing. Działa od razu.',
        'tooltip_urtx_rtaoscreenspace': 'Pozwala okluzji otoczenia ray tracing używać próbek z ekranu (domyślnie Wł.). '
                                        'Wył. używa tylko promieni. Wpływ na FPS zależy od sceny. Tylko zwykły Ray '
                                        'Tracing. Działa od razu.',
        'tooltip_urtx_rthbil': 'Dodaje światło pośrednie z przestrzeni ekranu (HBIL) do globalnego oświetlenia z ray '
                               'tracingiem. Wyłączone może zwiększyć FPS. Tylko zwykły Ray Tracing. Działa od razu.',
        'tooltip_urtx_rtreflfog': 'Nakłada mgłę w odbiciach ray tracing. Wył. daje wyraźniejsze odbicia we mgle i '
                                  'oszczędza trochę czasu GPU. Wyłączone może zwiększyć FPS. Zwykły Ray Tracing i Path '
                                  'Tracing. Działa od razu.',
        'tooltip_urtx_rtreflscreenspace': 'Pozwala odbiciom używać danych z przestrzeni ekranu przed śledzeniem '
                                          'promieni. Włączone może zwiększyć FPS. Tylko zwykły Ray Tracing. Działa od '
                                          'razu.',
        'tooltip_urtx_terrainpreload': 'Jak daleko teren jest wczytywany z wyprzedzeniem w kierunku ruchu (natywnie 20 '
                                       'm). Wyższe wartości zmniejszają doczytywanie obiektów, ale zwiększają '
                                       'ładowanie. Działa w nowo wczytanych obszarach.',
        'tooltip_urtx_texturestreams': 'Liczba tekstur wczytywanych jednocześnie (natywnie 12). Wyższe wartości '
                                       'zmniejszają doczytywanie obiektów, ale zwiększają ładowanie. Działa od razu.',
        'urt52_sep_cpu': ' CPU\xa0',
        'urt52_sep_fg': ' Frame Generation\xa0',
        'urt52_sep_pt': ' Path Tracing\xa0',
        'urt52_sep_rt': ' Ray Tracing\xa0',
        'urt52_sep_rtgip': ' RTGI: wydajność\xa0',
        'urt52_sep_rtgiq': ' RTGI: jakość\xa0',
        'urt52_sep_rtpt': ' RT/PT: wspólne\xa0',
        'urt52_sep_tex': ' Tekstury / renderowanie\xa0',
        'urtx_fgmenuresources': 'Frame Generation: Keep Resources in Menus',
        'urtx_n_0': '0',
        'urtx_n_0p005': '0.005',
        'urtx_n_0p015': '0.015',
        'urtx_n_0p02': '0.02',
        'urtx_n_0p03': '0.03',
        'urtx_n_0p05': '0.05',
        'urtx_n_0p1': '0.1',
        'urtx_n_0p25': '0.25',
        'urtx_n_0p5': '0.5',
        'urtx_n_0p75': '0.75',
        'urtx_n_1': '1',
        'urtx_n_10000_mb': '10000 MB',
        'urtx_n_100_m': '100 m',
        'urtx_n_10_m': '10 m',
        'urtx_n_16': '16',
        'urtx_n_1600_mb': '1600 MB',
        'urtx_n_2': '2',
        'urtx_n_2000_mb': '2000 MB',
        'urtx_n_200_m': '200 m',
        'urtx_n_24': '24',
        'urtx_n_3': '3',
        'urtx_n_3000_mb': '3000 MB',
        'urtx_n_32': '32',
        'urtx_n_4000_mb': '4000 MB',
        'urtx_n_400_m': '400 m',
        'urtx_n_40_m': '40 m',
        'urtx_n_48': '48',
        'urtx_n_6000_mb': '6000 MB',
        'urtx_n_600_m': '600 m',
        'urtx_n_64': '64',
        'urtx_n_8': '8',
        'urtx_n_8000_mb': '8000 MB',
        'urtx_n_minus0p5': '-0.5',
        'urtx_n_minus1': '-1',
        'urtx_n_minus1p5': '-1.5',
        'urtx_n_minus2': '-2',
        'urtx_ptaoquality': 'PT: Ambient Occlusion Quality',
        'urtx_ptskinbouncedesat': 'PT: Skin Bounce Light Desaturation',
        'urtx_rtaopretrace': 'RT: Ambient Occlusion Pre-Trace',
        'urtx_rtaoscreenspace': 'RT: Ambient Occlusion Screen-Space Sampling',
        'urtx_rthbil': 'RT: Screen-Space Indirect Lighting (HBIL)',
        'urtx_rtreflfog': 'RT/PT: Fog in Ray-Traced Reflections',
        'urtx_rtreflscreenspace': 'RT: Screen-Space Reflection Assist',
        'urtx_terrainpreload': 'Terrain Preload Distance',
        'urtx_texturestreams': 'Textures Streamed at Once',
        'urtx_w_keep': 'Zachowaj zasoby',
        'urtx_w_off': 'Wył.',
        'urtx_w_on': 'Wł.'},
 'ru': {'cr01_pt_scene_range': 'PT: Scene Range',
        'option_cr01_pt_scene_range': 'PT: Scene Range',
        'option_urt52_sep_cpu': ' CPU\xa0',
        'option_urt52_sep_fg': ' Frame Generation\xa0',
        'option_urt52_sep_pt': ' Path Tracing\xa0',
        'option_urt52_sep_rt': ' Ray Tracing\xa0',
        'option_urt52_sep_rtgip': ' RTGI: производительность\xa0',
        'option_urt52_sep_rtgiq': ' RTGI: качество\xa0',
        'option_urt52_sep_rtpt': ' RT/PT: общие\xa0',
        'option_urt52_sep_tex': ' Текстуры / рендеринг\xa0',
        'option_urtx_fgmenuresources': 'Frame Generation: Keep Resources in Menus',
        'option_urtx_ptaoquality': 'PT: Ambient Occlusion Quality',
        'option_urtx_ptskinbouncedesat': 'PT: Skin Bounce Light Desaturation',
        'option_urtx_rtaopretrace': 'RT: Ambient Occlusion Pre-Trace',
        'option_urtx_rtaoscreenspace': 'RT: Ambient Occlusion Screen-Space Sampling',
        'option_urtx_rthbil': 'RT: Screen-Space Indirect Lighting (HBIL)',
        'option_urtx_rtreflfog': 'RT/PT: Fog in Ray-Traced Reflections',
        'option_urtx_rtreflscreenspace': 'RT: Screen-Space Reflection Assist',
        'option_urtx_terrainpreload': 'Terrain Preload Distance',
        'option_urtx_texturestreams': 'Textures Streamed at Once',
        'preset_value_urt_m150': '150 m',
        'preset_value_urt_m200': '200 m',
        'preset_value_urt_m250': '250 m',
        'preset_value_urt_m300': '300 m',
        'tooltip_cr01_pt_scene_range': 'Уменьшает область трассировки вокруг вас (игра: 400 м); дальние отражения, '
                                       'непрямой свет и тени могут ослабнуть. Меньшие значения могут повысить FPS. '
                                       'Только Path Tracing. Применяется сразу.',
        'tooltip_urtx_fgmenuresources': 'Сохраняет ресурсы Frame Generation, пока открыто полноэкранное меню, что '
                                        'может избавить от рывка при возврате. Другие настройки Frame Generation, '
                                        'Reflex и V-Sync не меняются. Использует больше видеопамяти, пока открыто '
                                        'меню; FPS в игре не меняется. Применяется сразу.',
        'tooltip_urtx_ptaoquality': 'Качество фонового затенения при Path Tracing; низкие уровни шумнее. Меньшие '
                                    'значения могут повысить FPS. Только Path Tracing. Применяется сразу.',
        'tooltip_urtx_ptskinbouncedesat': 'Убирает цвет из света, отражённого на кожу; большие значения дают более '
                                          'нейтральный тон кожи. Дело вкуса; подтверждённого прироста FPS нет. Только '
                                          'Path Tracing. Применяется сразу.',
        'tooltip_urtx_rtaopretrace': 'Выполняет проход предварительной трассировки фонового затенения. Влияние на FPS '
                                     'зависит от сцены. Только обычная трассировка лучей. Применяется сразу.',
        'tooltip_urtx_rtaoscreenspace': 'Позволяет трассированному фоновому затенению использовать экранные выборки '
                                        '(по умолчанию Вкл.). Выкл. использует только лучи. Влияние на FPS зависит от '
                                        'сцены. Только обычная трассировка лучей. Применяется сразу.',
        'tooltip_urtx_rthbil': 'Добавляет непрямой свет экранного пространства (HBIL) к глобальному освещению с '
                               'трассировкой. Выкл. может повысить FPS. Только обычная трассировка лучей. Применяется '
                               'сразу.',
        'tooltip_urtx_rtreflfog': 'Применяет туман в трассированных отражениях. Выкл. даёт более чистые отражения в '
                                  'тумане и немного экономит время GPU. Выкл. может повысить FPS. Обычная трассировка '
                                  'лучей и Path Tracing. Применяется сразу.',
        'tooltip_urtx_rtreflscreenspace': 'Позволяет отражениям использовать данные экранного пространства до '
                                          'трассировки лучей. Вкл. может повысить FPS. Только обычная трассировка '
                                          'лучей. Применяется сразу.',
        'tooltip_urtx_terrainpreload': 'Насколько далеко вперёд подгружается ландшафт по направлению движения (по '
                                       'умолчанию 20 м). Большие значения уменьшают появление объектов на глазах, но '
                                       'добавляют загрузку. Применяется к заново загруженным областям.',
        'tooltip_urtx_texturestreams': 'Число текстур, загружаемых одновременно (по умолчанию 12). Большие значения '
                                       'уменьшают появление объектов на глазах, но добавляют загрузку. Применяется '
                                       'сразу.',
        'urt52_sep_cpu': ' CPU\xa0',
        'urt52_sep_fg': ' Frame Generation\xa0',
        'urt52_sep_pt': ' Path Tracing\xa0',
        'urt52_sep_rt': ' Ray Tracing\xa0',
        'urt52_sep_rtgip': ' RTGI: производительность\xa0',
        'urt52_sep_rtgiq': ' RTGI: качество\xa0',
        'urt52_sep_rtpt': ' RT/PT: общие\xa0',
        'urt52_sep_tex': ' Текстуры / рендеринг\xa0',
        'urtx_fgmenuresources': 'Frame Generation: Keep Resources in Menus',
        'urtx_n_0': '0',
        'urtx_n_0p005': '0.005',
        'urtx_n_0p015': '0.015',
        'urtx_n_0p02': '0.02',
        'urtx_n_0p03': '0.03',
        'urtx_n_0p05': '0.05',
        'urtx_n_0p1': '0.1',
        'urtx_n_0p25': '0.25',
        'urtx_n_0p5': '0.5',
        'urtx_n_0p75': '0.75',
        'urtx_n_1': '1',
        'urtx_n_10000_mb': '10000 MB',
        'urtx_n_100_m': '100 m',
        'urtx_n_10_m': '10 m',
        'urtx_n_16': '16',
        'urtx_n_1600_mb': '1600 MB',
        'urtx_n_2': '2',
        'urtx_n_2000_mb': '2000 MB',
        'urtx_n_200_m': '200 m',
        'urtx_n_24': '24',
        'urtx_n_3': '3',
        'urtx_n_3000_mb': '3000 MB',
        'urtx_n_32': '32',
        'urtx_n_4000_mb': '4000 MB',
        'urtx_n_400_m': '400 m',
        'urtx_n_40_m': '40 m',
        'urtx_n_48': '48',
        'urtx_n_6000_mb': '6000 MB',
        'urtx_n_600_m': '600 m',
        'urtx_n_64': '64',
        'urtx_n_8': '8',
        'urtx_n_8000_mb': '8000 MB',
        'urtx_n_minus0p5': '-0.5',
        'urtx_n_minus1': '-1',
        'urtx_n_minus1p5': '-1.5',
        'urtx_n_minus2': '-2',
        'urtx_ptaoquality': 'PT: Ambient Occlusion Quality',
        'urtx_ptskinbouncedesat': 'PT: Skin Bounce Light Desaturation',
        'urtx_rtaopretrace': 'RT: Ambient Occlusion Pre-Trace',
        'urtx_rtaoscreenspace': 'RT: Ambient Occlusion Screen-Space Sampling',
        'urtx_rthbil': 'RT: Screen-Space Indirect Lighting (HBIL)',
        'urtx_rtreflfog': 'RT/PT: Fog in Ray-Traced Reflections',
        'urtx_rtreflscreenspace': 'RT: Screen-Space Reflection Assist',
        'urtx_terrainpreload': 'Terrain Preload Distance',
        'urtx_texturestreams': 'Textures Streamed at Once',
        'urtx_w_keep': 'Сохранять ресурсы',
        'urtx_w_off': 'Выкл.',
        'urtx_w_on': 'Вкл.'},
 'tr': {'cr01_pt_scene_range': 'PT: Scene Range',
        'option_cr01_pt_scene_range': 'PT: Scene Range',
        'option_urt52_sep_cpu': ' CPU\xa0',
        'option_urt52_sep_fg': ' Frame Generation\xa0',
        'option_urt52_sep_pt': ' Path Tracing\xa0',
        'option_urt52_sep_rt': ' Ray Tracing\xa0',
        'option_urt52_sep_rtgip': ' RTGI: performans\xa0',
        'option_urt52_sep_rtgiq': ' RTGI: kalite\xa0',
        'option_urt52_sep_rtpt': ' RT/PT: ortak\xa0',
        'option_urt52_sep_tex': ' Dokular / işleme\xa0',
        'option_urtx_fgmenuresources': 'Frame Generation: Keep Resources in Menus',
        'option_urtx_ptaoquality': 'PT: Ambient Occlusion Quality',
        'option_urtx_ptskinbouncedesat': 'PT: Skin Bounce Light Desaturation',
        'option_urtx_rtaopretrace': 'RT: Ambient Occlusion Pre-Trace',
        'option_urtx_rtaoscreenspace': 'RT: Ambient Occlusion Screen-Space Sampling',
        'option_urtx_rthbil': 'RT: Screen-Space Indirect Lighting (HBIL)',
        'option_urtx_rtreflfog': 'RT/PT: Fog in Ray-Traced Reflections',
        'option_urtx_rtreflscreenspace': 'RT: Screen-Space Reflection Assist',
        'option_urtx_terrainpreload': 'Terrain Preload Distance',
        'option_urtx_texturestreams': 'Textures Streamed at Once',
        'preset_value_urt_m150': '150 m',
        'preset_value_urt_m200': '200 m',
        'preset_value_urt_m250': '250 m',
        'preset_value_urt_m300': '300 m',
        'tooltip_cr01_pt_scene_range': 'Etrafındaki ışın izlenen alanı küçültür (oyun: 400 m); uzak yansımalar, '
                                       "dolaylı ışık ve gölgeler azalabilir. Düşük değerler FPS'i artırabilir. "
                                       'Yalnızca Path Tracing. Hemen uygulanır.',
        'tooltip_urtx_fgmenuresources': 'Tam ekran bir menü açıkken Frame Generation kaynaklarını korur; bu, geri '
                                        'dönüşteki takılmayı önleyebilir. Diğer Frame Generation, Reflex ve V-Sync '
                                        'ayarları değişmez. Menü açıkken daha fazla video belleği kullanır; oyun '
                                        'sırasında FPS değişmez. Hemen uygulanır.',
        'tooltip_urtx_ptaoquality': 'Path tracing ortam kapatmasının kalitesi; düşük seviyeler daha gürültülüdür. '
                                    "Düşük değerler FPS'i artırabilir. Yalnızca Path Tracing. Hemen uygulanır.",
        'tooltip_urtx_ptskinbouncedesat': 'Cilde seken ışıktan rengi alır; yüksek değerler daha nötr ten tonları '
                                          'verir. Görsel tercih; kanıtlanmış FPS kazancı yok. Yalnızca Path Tracing. '
                                          'Hemen uygulanır.',
        'tooltip_urtx_rtaopretrace': 'Işın izlemeli ortam kapatmanın ön izleme geçişini çalıştırır. FPS etkisi sahneye '
                                     'bağlıdır. Yalnızca normal Ray Tracing. Hemen uygulanır.',
        'tooltip_urtx_rtaoscreenspace': 'Işın izlemeli ortam perdelemesinin ekran örneklerini kullanmasına izin verir '
                                        '(varsayılan Açık). Kapalı yalnızca ışınları kullanır. FPS etkisi sahneye '
                                        'bağlıdır. Yalnızca normal Ray Tracing. Hemen uygulanır.',
        'tooltip_urtx_rthbil': 'Işın izlemeli küresel aydınlatmaya ekran alanı dolaylı ışığı (HBIL) ekler. Kapalı '
                               "FPS'i artırabilir. Yalnızca normal Ray Tracing. Hemen uygulanır.",
        'tooltip_urtx_rtreflfog': 'Işın izlemeli yansımaların içinde sis uygular. Kapalı, sisli havada daha net '
                                  "yansımalar verir ve biraz GPU süresi kazandırır. Kapalı FPS'i artırabilir. Normal "
                                  'Ray Tracing ve Path Tracing. Hemen uygulanır.',
        'tooltip_urtx_rtreflscreenspace': 'Yansımaların ışın izlemeden önce ekran alanı verilerini kullanmasına izin '
                                          "verir. Açık FPS'i artırabilir. Yalnızca normal Ray Tracing. Hemen "
                                          'uygulanır.',
        'tooltip_urtx_terrainpreload': 'Arazinin ilerleme yönünüzde ne kadar önceden yüklendiği (yerel 20 m). Yüksek '
                                       'değerler nesnelerin aniden belirmesini azaltır ancak yükleme işini artırır. '
                                       'Yeni yüklenen alanlara uygulanır.',
        'tooltip_urtx_texturestreams': 'Aynı anda yüklenen doku sayısı (yerel 12). Yüksek değerler nesnelerin aniden '
                                       'belirmesini azaltır ancak yükleme işini artırır. Hemen uygulanır.',
        'urt52_sep_cpu': ' CPU\xa0',
        'urt52_sep_fg': ' Frame Generation\xa0',
        'urt52_sep_pt': ' Path Tracing\xa0',
        'urt52_sep_rt': ' Ray Tracing\xa0',
        'urt52_sep_rtgip': ' RTGI: performans\xa0',
        'urt52_sep_rtgiq': ' RTGI: kalite\xa0',
        'urt52_sep_rtpt': ' RT/PT: ortak\xa0',
        'urt52_sep_tex': ' Dokular / işleme\xa0',
        'urtx_fgmenuresources': 'Frame Generation: Keep Resources in Menus',
        'urtx_n_0': '0',
        'urtx_n_0p005': '0.005',
        'urtx_n_0p015': '0.015',
        'urtx_n_0p02': '0.02',
        'urtx_n_0p03': '0.03',
        'urtx_n_0p05': '0.05',
        'urtx_n_0p1': '0.1',
        'urtx_n_0p25': '0.25',
        'urtx_n_0p5': '0.5',
        'urtx_n_0p75': '0.75',
        'urtx_n_1': '1',
        'urtx_n_10000_mb': '10000 MB',
        'urtx_n_100_m': '100 m',
        'urtx_n_10_m': '10 m',
        'urtx_n_16': '16',
        'urtx_n_1600_mb': '1600 MB',
        'urtx_n_2': '2',
        'urtx_n_2000_mb': '2000 MB',
        'urtx_n_200_m': '200 m',
        'urtx_n_24': '24',
        'urtx_n_3': '3',
        'urtx_n_3000_mb': '3000 MB',
        'urtx_n_32': '32',
        'urtx_n_4000_mb': '4000 MB',
        'urtx_n_400_m': '400 m',
        'urtx_n_40_m': '40 m',
        'urtx_n_48': '48',
        'urtx_n_6000_mb': '6000 MB',
        'urtx_n_600_m': '600 m',
        'urtx_n_64': '64',
        'urtx_n_8': '8',
        'urtx_n_8000_mb': '8000 MB',
        'urtx_n_minus0p5': '-0.5',
        'urtx_n_minus1': '-1',
        'urtx_n_minus1p5': '-1.5',
        'urtx_n_minus2': '-2',
        'urtx_ptaoquality': 'PT: Ambient Occlusion Quality',
        'urtx_ptskinbouncedesat': 'PT: Skin Bounce Light Desaturation',
        'urtx_rtaopretrace': 'RT: Ambient Occlusion Pre-Trace',
        'urtx_rtaoscreenspace': 'RT: Ambient Occlusion Screen-Space Sampling',
        'urtx_rthbil': 'RT: Screen-Space Indirect Lighting (HBIL)',
        'urtx_rtreflfog': 'RT/PT: Fog in Ray-Traced Reflections',
        'urtx_rtreflscreenspace': 'RT: Screen-Space Reflection Assist',
        'urtx_terrainpreload': 'Terrain Preload Distance',
        'urtx_texturestreams': 'Textures Streamed at Once',
        'urtx_w_keep': 'Kaynakları koru',
        'urtx_w_off': 'Kapalı',
        'urtx_w_on': 'Açık'},
 'ua': {'cr01_pt_scene_range': 'PT: Scene Range',
        'option_cr01_pt_scene_range': 'PT: Scene Range',
        'option_urt52_sep_cpu': ' CPU\xa0',
        'option_urt52_sep_fg': ' Frame Generation\xa0',
        'option_urt52_sep_pt': ' Path Tracing\xa0',
        'option_urt52_sep_rt': ' Ray Tracing\xa0',
        'option_urt52_sep_rtgip': ' RTGI: продуктивність\xa0',
        'option_urt52_sep_rtgiq': ' RTGI: якість\xa0',
        'option_urt52_sep_rtpt': ' RT/PT: спільні\xa0',
        'option_urt52_sep_tex': ' Текстури / рендеринг\xa0',
        'option_urtx_fgmenuresources': 'Frame Generation: Keep Resources in Menus',
        'option_urtx_ptaoquality': 'PT: Ambient Occlusion Quality',
        'option_urtx_ptskinbouncedesat': 'PT: Skin Bounce Light Desaturation',
        'option_urtx_rtaopretrace': 'RT: Ambient Occlusion Pre-Trace',
        'option_urtx_rtaoscreenspace': 'RT: Ambient Occlusion Screen-Space Sampling',
        'option_urtx_rthbil': 'RT: Screen-Space Indirect Lighting (HBIL)',
        'option_urtx_rtreflfog': 'RT/PT: Fog in Ray-Traced Reflections',
        'option_urtx_rtreflscreenspace': 'RT: Screen-Space Reflection Assist',
        'option_urtx_terrainpreload': 'Terrain Preload Distance',
        'option_urtx_texturestreams': 'Textures Streamed at Once',
        'preset_value_urt_m150': '150 m',
        'preset_value_urt_m200': '200 m',
        'preset_value_urt_m250': '250 m',
        'preset_value_urt_m300': '300 m',
        'tooltip_cr01_pt_scene_range': 'Зменшує область трасування навколо вас (гра: 400 м); далекі відображення, '
                                       'непряме світло й тіні можуть послабшати. Менші значення можуть підвищити FPS. '
                                       'Лише Path Tracing. Застосовується одразу.',
        'tooltip_urtx_fgmenuresources': 'Зберігає ресурси Frame Generation, поки відкрите повноекранне меню, що може '
                                        'усунути ривок під час повернення. Інші налаштування Frame Generation, Reflex '
                                        "і V-Sync не змінюються. Використовує більше відеопам'яті, поки відкрите меню; "
                                        'FPS у грі не змінюється. Застосовується одразу.',
        'tooltip_urtx_ptaoquality': 'Якість фонового затінення при Path Tracing; низькі рівні шумніші. Менші значення '
                                    'можуть підвищити FPS. Лише Path Tracing. Застосовується одразу.',
        'tooltip_urtx_ptskinbouncedesat': 'Прибирає колір зі світла, відбитого на шкіру; більші значення дають '
                                          'нейтральніший тон шкіри. Справа смаку; підтвердженого приросту FPS немає. '
                                          'Лише Path Tracing. Застосовується одразу.',
        'tooltip_urtx_rtaopretrace': 'Виконує прохід попереднього трасування фонового затінення. Вплив на FPS залежить '
                                     'від сцени. Лише звичайне трасування променів. Застосовується одразу.',
        'tooltip_urtx_rtaoscreenspace': 'Дозволяє трасованому фоновому затіненню використовувати екранні вибірки '
                                        '(типово Увімк.). Вимк. використовує лише промені. Вплив на FPS залежить від '
                                        'сцени. Лише звичайне трасування променів. Застосовується одразу.',
        'tooltip_urtx_rthbil': 'Додає непряме світло екранного простору (HBIL) до глобального освітлення з '
                               'трасуванням. Вимк. може підвищити FPS. Лише звичайне трасування променів. '
                               'Застосовується одразу.',
        'tooltip_urtx_rtreflfog': 'Застосовує туман у трасованих відображеннях. Вимк. дає чистіші відображення в '
                                  'тумані й трохи заощаджує час GPU. Вимк. може підвищити FPS. Звичайне трасування '
                                  'променів і Path Tracing. Застосовується одразу.',
        'tooltip_urtx_rtreflscreenspace': 'Дозволяє відображенням використовувати дані екранного простору до '
                                          'трасування променів. Увімк. може підвищити FPS. Лише звичайне трасування '
                                          'променів. Застосовується одразу.',
        'tooltip_urtx_terrainpreload': 'Наскільки далеко вперед підвантажується ландшафт у напрямку руху (типово 20 '
                                       "м). Більші значення зменшують появу об'єктів на очах, але додають "
                                       'завантаження. Застосовується до щойно завантажених областей.',
        'tooltip_urtx_texturestreams': 'Кількість текстур, що завантажуються одночасно (типово 12). Більші значення '
                                       "зменшують появу об'єктів на очах, але додають завантаження. Застосовується "
                                       'одразу.',
        'urt52_sep_cpu': ' CPU\xa0',
        'urt52_sep_fg': ' Frame Generation\xa0',
        'urt52_sep_pt': ' Path Tracing\xa0',
        'urt52_sep_rt': ' Ray Tracing\xa0',
        'urt52_sep_rtgip': ' RTGI: продуктивність\xa0',
        'urt52_sep_rtgiq': ' RTGI: якість\xa0',
        'urt52_sep_rtpt': ' RT/PT: спільні\xa0',
        'urt52_sep_tex': ' Текстури / рендеринг\xa0',
        'urtx_fgmenuresources': 'Frame Generation: Keep Resources in Menus',
        'urtx_n_0': '0',
        'urtx_n_0p005': '0.005',
        'urtx_n_0p015': '0.015',
        'urtx_n_0p02': '0.02',
        'urtx_n_0p03': '0.03',
        'urtx_n_0p05': '0.05',
        'urtx_n_0p1': '0.1',
        'urtx_n_0p25': '0.25',
        'urtx_n_0p5': '0.5',
        'urtx_n_0p75': '0.75',
        'urtx_n_1': '1',
        'urtx_n_10000_mb': '10000 MB',
        'urtx_n_100_m': '100 m',
        'urtx_n_10_m': '10 m',
        'urtx_n_16': '16',
        'urtx_n_1600_mb': '1600 MB',
        'urtx_n_2': '2',
        'urtx_n_2000_mb': '2000 MB',
        'urtx_n_200_m': '200 m',
        'urtx_n_24': '24',
        'urtx_n_3': '3',
        'urtx_n_3000_mb': '3000 MB',
        'urtx_n_32': '32',
        'urtx_n_4000_mb': '4000 MB',
        'urtx_n_400_m': '400 m',
        'urtx_n_40_m': '40 m',
        'urtx_n_48': '48',
        'urtx_n_6000_mb': '6000 MB',
        'urtx_n_600_m': '600 m',
        'urtx_n_64': '64',
        'urtx_n_8': '8',
        'urtx_n_8000_mb': '8000 MB',
        'urtx_n_minus0p5': '-0.5',
        'urtx_n_minus1': '-1',
        'urtx_n_minus1p5': '-1.5',
        'urtx_n_minus2': '-2',
        'urtx_ptaoquality': 'PT: Ambient Occlusion Quality',
        'urtx_ptskinbouncedesat': 'PT: Skin Bounce Light Desaturation',
        'urtx_rtaopretrace': 'RT: Ambient Occlusion Pre-Trace',
        'urtx_rtaoscreenspace': 'RT: Ambient Occlusion Screen-Space Sampling',
        'urtx_rthbil': 'RT: Screen-Space Indirect Lighting (HBIL)',
        'urtx_rtreflfog': 'RT/PT: Fog in Ray-Traced Reflections',
        'urtx_rtreflscreenspace': 'RT: Screen-Space Reflection Assist',
        'urtx_terrainpreload': 'Terrain Preload Distance',
        'urtx_texturestreams': 'Textures Streamed at Once',
        'urtx_w_keep': 'Зберігати ресурси',
        'urtx_w_off': 'Вимк.',
        'urtx_w_on': 'Увімк.'},
 'zh': {'cr01_pt_scene_range': 'PT: Scene Range',
        'option_cr01_pt_scene_range': 'PT: Scene Range',
        'option_urt52_sep_cpu': ' CPU\xa0',
        'option_urt52_sep_fg': ' Frame Generation\xa0',
        'option_urt52_sep_pt': ' Path Tracing\xa0',
        'option_urt52_sep_rt': ' Ray Tracing\xa0',
        'option_urt52_sep_rtgip': ' RTGI：效能\xa0',
        'option_urt52_sep_rtgiq': ' RTGI：品質\xa0',
        'option_urt52_sep_rtpt': ' RT/PT：共用\xa0',
        'option_urt52_sep_tex': ' 材質貼圖 / 渲染\xa0',
        'option_urtx_fgmenuresources': 'Frame Generation: Keep Resources in Menus',
        'option_urtx_ptaoquality': 'PT: Ambient Occlusion Quality',
        'option_urtx_ptskinbouncedesat': 'PT: Skin Bounce Light Desaturation',
        'option_urtx_rtaopretrace': 'RT: Ambient Occlusion Pre-Trace',
        'option_urtx_rtaoscreenspace': 'RT: Ambient Occlusion Screen-Space Sampling',
        'option_urtx_rthbil': 'RT: Screen-Space Indirect Lighting (HBIL)',
        'option_urtx_rtreflfog': 'RT/PT: Fog in Ray-Traced Reflections',
        'option_urtx_rtreflscreenspace': 'RT: Screen-Space Reflection Assist',
        'option_urtx_terrainpreload': 'Terrain Preload Distance',
        'option_urtx_texturestreams': 'Textures Streamed at Once',
        'preset_value_urt_m150': '150 m',
        'preset_value_urt_m200': '200 m',
        'preset_value_urt_m250': '250 m',
        'preset_value_urt_m300': '300 m',
        'tooltip_cr01_pt_scene_range': '縮小你周圍的光線追蹤範圍（遊戲預設：400 m）；遠處的反射、間接光與陰影可能減弱。降低數值可能提升 FPS。僅限 Path Tracing。立即生效。',
        'tooltip_urtx_fgmenuresources': '在全螢幕選單開啟期間保留 Frame Generation 資源，可能避免返回遊戲時的卡頓。不會變更 Frame Generation、Reflex 或 '
                                        'V-Sync 的其他設定。選單開啟時使用更多顯示記憶體；遊戲中 FPS 不變。立即生效。',
        'tooltip_urtx_ptaoquality': '路徑追蹤環境光遮蔽的品質；等級越低雜訊越多。降低數值可能提升 FPS。僅限 Path Tracing。立即生效。',
        'tooltip_urtx_ptskinbouncedesat': '去除反彈到皮膚上的光線顏色；數值越高膚色越中性。屬於視覺偏好，沒有已確認的 FPS 效益。僅限 Path Tracing。立即生效。',
        'tooltip_urtx_rtaopretrace': '執行光線追蹤環境光遮蔽的預追蹤。對 FPS 的影響取決於場景。僅限一般 Ray Tracing。立即生效。',
        'tooltip_urtx_rtaoscreenspace': '允許光線追蹤環境光遮蔽使用螢幕取樣（預設開）。關則只使用光線。對 FPS 的影響取決於場景。僅限一般 Ray Tracing。立即生效。',
        'tooltip_urtx_rthbil': '在光線追蹤全域光照上疊加螢幕空間間接光（HBIL）。關閉可能提升 FPS。僅限一般 Ray Tracing。立即生效。',
        'tooltip_urtx_rtreflfog': '在光線追蹤反射中套用霧。關會讓霧天的反射更清晰，並節省少量 GPU 時間。關閉可能提升 FPS。一般 Ray Tracing 與 Path Tracing。立即生效。',
        'tooltip_urtx_rtreflscreenspace': '在追蹤光線前讓反射利用螢幕空間資料。開啟可能提升 FPS。僅限一般 Ray Tracing。立即生效。',
        'tooltip_urtx_terrainpreload': '沿行進方向提前載入地形的距離（原生 20 m）。提高數值可減少物件突然出現，但會增加載入工作。對新載入的區域生效。',
        'tooltip_urtx_texturestreams': '同時載入的材質數量（原生 12）。提高數值可減少物件突然出現，但會增加載入工作。立即生效。',
        'urt52_sep_cpu': ' CPU\xa0',
        'urt52_sep_fg': ' Frame Generation\xa0',
        'urt52_sep_pt': ' Path Tracing\xa0',
        'urt52_sep_rt': ' Ray Tracing\xa0',
        'urt52_sep_rtgip': ' RTGI：效能\xa0',
        'urt52_sep_rtgiq': ' RTGI：品質\xa0',
        'urt52_sep_rtpt': ' RT/PT：共用\xa0',
        'urt52_sep_tex': ' 材質貼圖 / 渲染\xa0',
        'urtx_fgmenuresources': 'Frame Generation: Keep Resources in Menus',
        'urtx_n_0': '0',
        'urtx_n_0p005': '0.005',
        'urtx_n_0p015': '0.015',
        'urtx_n_0p02': '0.02',
        'urtx_n_0p03': '0.03',
        'urtx_n_0p05': '0.05',
        'urtx_n_0p1': '0.1',
        'urtx_n_0p25': '0.25',
        'urtx_n_0p5': '0.5',
        'urtx_n_0p75': '0.75',
        'urtx_n_1': '1',
        'urtx_n_10000_mb': '10000 MB',
        'urtx_n_100_m': '100 m',
        'urtx_n_10_m': '10 m',
        'urtx_n_16': '16',
        'urtx_n_1600_mb': '1600 MB',
        'urtx_n_2': '2',
        'urtx_n_2000_mb': '2000 MB',
        'urtx_n_200_m': '200 m',
        'urtx_n_24': '24',
        'urtx_n_3': '3',
        'urtx_n_3000_mb': '3000 MB',
        'urtx_n_32': '32',
        'urtx_n_4000_mb': '4000 MB',
        'urtx_n_400_m': '400 m',
        'urtx_n_40_m': '40 m',
        'urtx_n_48': '48',
        'urtx_n_6000_mb': '6000 MB',
        'urtx_n_600_m': '600 m',
        'urtx_n_64': '64',
        'urtx_n_8': '8',
        'urtx_n_8000_mb': '8000 MB',
        'urtx_n_minus0p5': '-0.5',
        'urtx_n_minus1': '-1',
        'urtx_n_minus1p5': '-1.5',
        'urtx_n_minus2': '-2',
        'urtx_ptaoquality': 'PT: Ambient Occlusion Quality',
        'urtx_ptskinbouncedesat': 'PT: Skin Bounce Light Desaturation',
        'urtx_rtaopretrace': 'RT: Ambient Occlusion Pre-Trace',
        'urtx_rtaoscreenspace': 'RT: Ambient Occlusion Screen-Space Sampling',
        'urtx_rthbil': 'RT: Screen-Space Indirect Lighting (HBIL)',
        'urtx_rtreflfog': 'RT/PT: Fog in Ray-Traced Reflections',
        'urtx_rtreflscreenspace': 'RT: Screen-Space Reflection Assist',
        'urtx_terrainpreload': 'Terrain Preload Distance',
        'urtx_texturestreams': 'Textures Streamed at Once',
        'urtx_w_keep': '保留資源',
        'urtx_w_off': '關',
        'urtx_w_on': '開'}}
CORE_TIPS = {'ar': {'tooltip_cr01_ambientocclusion': 'الحجب المحيطي الخاص باللعبة. الإيقاف قد يحسن FPS. Path Tracing فقط. يُطبق '
                                         'فورًا.',
        'tooltip_cr01_aocontrast': 'تباين الحجب المحيطي. تفضيل بصري؛ لا توجد فائدة مثبتة في FPS. Path Tracing فقط. '
                                   'يُطبق فورًا.',
        'tooltip_cr01_aoradius': 'مدى امتداد الحجب المحيطي. تفضيل بصري؛ لا توجد فائدة مثبتة في FPS. Path Tracing فقط. '
                                 'يُطبق فورًا.',
        'tooltip_cr01_aostrength': 'مدى عتمة الحجب المحيطي. تفضيل بصري؛ لا توجد فائدة مثبتة في FPS. Path Tracing فقط. '
                                   'يُطبق فورًا.',
        'tooltip_cr01_cpu_distant_trees': 'يحسّن لوحات الأشجار البعيدة. يتوقف مؤقتًا في Path Tracing ما دام PT: '
                                          'Billboards مفعّلًا. التشغيل قد يحسن FPS. تتبع الأشعة العادي و Path Tracing. '
                                          'يُطبق على المناطق المحملة حديثًا.',
        'tooltip_cr01_cpu_fast_bone_reads': 'يسرّع قراءة المعالج لعظام تحريك الشخصيات والمخلوقات؛ يبقى شكل الحركة كما '
                                            'هو. التشغيل قد يحسن FPS. يُطبق فورًا.',
        'tooltip_cr01_cpu_physics_lod': 'يقلل عمل المعالج على تفاصيل الفيزياء؛ قد تتحرك الأجسام البعيدة والأقمشة '
                                        'بتفاصيل أقل. التشغيل قد يحسن FPS. يُطبق فورًا.',
        'tooltip_cr01_geometrybounces': 'عدد مرات ارتداد الضوء عن الأسطح. القيم الأقل قد تحسن FPS. Path Tracing فقط. '
                                        'يُطبق فورًا.',
        'tooltip_cr01_grassday': 'سطوع العشب نهارًا. تفضيل بصري؛ لا توجد فائدة مثبتة في FPS. Path Tracing فقط. يُطبق '
                                 'فورًا.',
        'tooltip_cr01_grassnight': 'سطوع العشب ليلًا. تفضيل بصري؛ لا توجد فائدة مثبتة في FPS. Path Tracing فقط. يُطبق '
                                   'فورًا.',
        'tooltip_cr01_hairbounces': 'عدد مرات ارتداد الضوء داخل الشعر. القيم الأقل قد تحسن FPS. Path Tracing فقط. '
                                    'يُطبق فورًا.',
        'tooltip_cr01_indirectlight': 'سطوع الضوء غير المباشر (1.00 = محايد). تفضيل بصري؛ لا توجد فائدة مثبتة في FPS. '
                                      'Path Tracing فقط. يُطبق فورًا.',
        'tooltip_cr01_pt_scene_range': 'يصغّر منطقة تتبع الأشعة حولك (اللعبة: 400 م)؛ قد تضعف الانعكاسات والإضاءة غير '
                                       'المباشرة والظلال البعيدة. القيم الأقل قد تحسن FPS. Path Tracing فقط. يُطبق '
                                       'فورًا.',
        'tooltip_cr01_ptbillboards': 'ظلال لوحات الأشجار البعيدة. التشغيل يوقف CPU: Distant Trees مؤقتًا. الإيقاف قد '
                                     'يحسن FPS. Path Tracing فقط. يُطبق على المناطق المحملة حديثًا.',
        'tooltip_cr01_ptshadows': 'ظلال تتبع الأشعة؛ الإيقاف يفقد تفاصيل الظل. الإيقاف قد يحسن FPS. Path Tracing فقط. '
                                  'يُطبق فورًا.',
        'tooltip_cr01_rt_perf_ao': 'جودة التتبع المسبق لظلال التلامس. القيم الأقل قد تحسن FPS. تتبع الأشعة العادي فقط. '
                                   'يُطبق فورًا. يتطلب ضبط Ray-Traced Optimizations على Auto.',
        'tooltip_cr01_rt_perf_budget': 'عمل الإضاءة لكل إطار؛ الميزانية المنخفضة قد تؤخر الضوء أو تبهت العشب. القيم '
                                       'الأقل قد تحسن FPS. تتبع الأشعة العادي فقط. يُطبق فورًا. يتطلب ضبط Ray-Traced '
                                       'Optimizations على Auto.',
        'tooltip_cr01_rt_perf_cap': 'يحد عدد مجسات الضوء المصنفة؛ الحدود المنخفضة قد تؤخر تحديث الإضاءة. القيم الأقل '
                                    'قد تحسن FPS. تتبع الأشعة العادي فقط. يُطبق فورًا. يتطلب ضبط Ray-Traced '
                                    'Optimizations على Auto.',
        'tooltip_cr01_rt_perf_global_shading_limit': 'مجسات الشبكة العامة المظللة لكل إطار؛ الحدود المنخفضة قد تؤخر '
                                                     'تحديث الإضاءة. القيم الأقل قد تحسن FPS. تتبع الأشعة العادي فقط. '
                                                     'يُطبق فورًا. يتطلب ضبط Ray-Traced Optimizations على Auto.',
        'tooltip_cr01_rt_perf_grid1_shading_limit': 'مجسات الشبكة الثانوية المظللة لكل إطار؛ الحدود المنخفضة قد تؤخر '
                                                    'تحديث الإضاءة. القيم الأقل قد تحسن FPS. تتبع الأشعة العادي فقط. '
                                                    'يُطبق فورًا. يتطلب ضبط Ray-Traced Optimizations على Auto.',
        'tooltip_cr01_rt_perf_primary': 'أشعة التصنيف الأولية للإضاءة الشاملة؛ أشعة أقل قد تغير الإضاءة. القيم الأقل '
                                        'قد تحسن FPS. تتبع الأشعة العادي فقط. يُطبق فورًا. يتطلب ضبط Ray-Traced '
                                        'Optimizations على Auto.',
        'tooltip_cr01_rt_perf_secondary': 'أشعة التصنيف الثانوية للإضاءة الشاملة؛ أشعة أقل قد تغير الإضاءة. القيم '
                                          'الأقل قد تحسن FPS. تتبع الأشعة العادي فقط. يُطبق فورًا. يتطلب ضبط '
                                          'Ray-Traced Optimizations على Auto.',
        'tooltip_cr01_rt_quality_ao': 'جودة التتبع المسبق لظلال التلامس. القيم الأقل قد تحسن FPS. تتبع الأشعة العادي '
                                      'فقط. يُطبق فورًا. يتطلب ضبط Ray-Traced Optimizations على Auto.',
        'tooltip_cr01_rt_quality_budget': 'عمل الإضاءة لكل إطار؛ الميزانية المنخفضة قد تؤخر الضوء أو تبهت العشب. القيم '
                                          'الأقل قد تحسن FPS. تتبع الأشعة العادي فقط. يُطبق فورًا. يتطلب ضبط '
                                          'Ray-Traced Optimizations على Auto.',
        'tooltip_cr01_rt_quality_cap': 'يحد عدد مجسات الضوء المصنفة؛ الحدود المنخفضة قد تؤخر تحديث الإضاءة. القيم '
                                       'الأقل قد تحسن FPS. تتبع الأشعة العادي فقط. يُطبق فورًا. يتطلب ضبط Ray-Traced '
                                       'Optimizations على Auto.',
        'tooltip_cr01_rt_quality_global_shading_limit': 'مجسات الشبكة العامة المظللة لكل إطار؛ الحدود المنخفضة قد تؤخر '
                                                        'تحديث الإضاءة. القيم الأقل قد تحسن FPS. تتبع الأشعة العادي '
                                                        'فقط. يُطبق فورًا. يتطلب ضبط Ray-Traced Optimizations على '
                                                        'Auto.',
        'tooltip_cr01_rt_quality_grid1_shading_limit': 'مجسات الشبكة الثانوية المظللة لكل إطار؛ الحدود المنخفضة قد '
                                                       'تؤخر تحديث الإضاءة. القيم الأقل قد تحسن FPS. تتبع الأشعة '
                                                       'العادي فقط. يُطبق فورًا. يتطلب ضبط Ray-Traced Optimizations '
                                                       'على Auto.',
        'tooltip_cr01_rt_quality_primary': 'أشعة التصنيف الأولية للإضاءة الشاملة؛ أشعة أقل قد تغير الإضاءة. القيم '
                                           'الأقل قد تحسن FPS. تتبع الأشعة العادي فقط. يُطبق فورًا. يتطلب ضبط '
                                           'Ray-Traced Optimizations على Auto.',
        'tooltip_cr01_rt_quality_secondary': 'أشعة التصنيف الثانوية للإضاءة الشاملة؛ أشعة أقل قد تغير الإضاءة. القيم '
                                             'الأقل قد تحسن FPS. تتبع الأشعة العادي فقط. يُطبق فورًا. يتطلب ضبط '
                                             'Ray-Traced Optimizations على Auto.',
        'tooltip_cr01_rt_refl_local_lights': 'الإيقاف يتجاهل المصابيح والمشاعل والنيران عند إضاءة الانعكاسات؛ تبدو '
                                             'المناطق المضاءة أغمق فيها. الإيقاف قد يحسن FPS. تتبع الأشعة العادي فقط. '
                                             'يُطبق فورًا.',
        'tooltip_cr01_rt_shadow_foliage_detail': 'يستخدم خامات أوراق أخشن لأشعة الظل؛ تصبح ظلال الأوراق أنعم. تفاصيل '
                                                 'أقل قد تحسن FPS. تتبع الأشعة العادي فقط. يُطبق فورًا.',
        'tooltip_cr01_rt_shadow_local_lights': 'عدد الأضواء القريبة التي تلقي ظلال تتبع الأشعة (اللعبة: 8)؛ ظلال الشمس '
                                               'لا تتغير. القيم الأقل قد تحسن FPS. تتبع الأشعة العادي فقط. يُطبق '
                                               'فورًا.',
        'tooltip_cr01_skyimpact': 'قوة ضوء السماء (1.00 = محايد). تفضيل بصري؛ لا توجد فائدة مثبتة في FPS. Path Tracing '
                                  'فقط. يُطبق فورًا.',
        'tooltip_cr01_specularcheckerboard': 'أخذ عينات بنمط رقعة الشطرنج لبعض الانعكاسات؛ التشغيل قد يفقد قليلًا من '
                                             'التفاصيل. التشغيل قد يحسن FPS. Path Tracing فقط. يُطبق فورًا.',
        'tooltip_cr01_specularscreenspace': 'مساعدة في فضاء الشاشة لبعض الانعكاسات؛ الإيقاف قد يفقد بعض التفاصيل. '
                                            'الإيقاف قد يحسن FPS. Path Tracing فقط. يُطبق فورًا.',
        'tooltip_cr01_sunimpact': 'قوة ضوء الشمس (1.00 = محايد). تفضيل بصري؛ لا توجد فائدة مثبتة في FPS. Path Tracing '
                                  'فقط. يُطبق فورًا.'},
 'br': {'tooltip_cr01_ambientocclusion': 'A oclusão ambiente do jogo. Desligado pode aumentar o FPS. Apenas Path '
                                         'Tracing. Aplica-se imediatamente.',
        'tooltip_cr01_aocontrast': 'Contraste da oclusão ambiente. Preferência visual; sem ganho de FPS comprovado. '
                                   'Apenas Path Tracing. Aplica-se imediatamente.',
        'tooltip_cr01_aoradius': 'Até onde a oclusão ambiente alcança. Preferência visual; sem ganho de FPS '
                                 'comprovado. Apenas Path Tracing. Aplica-se imediatamente.',
        'tooltip_cr01_aostrength': 'O quanto a oclusão ambiente escurece. Preferência visual; sem ganho de FPS '
                                   'comprovado. Apenas Path Tracing. Aplica-se imediatamente.',
        'tooltip_cr01_cpu_distant_trees': 'Otimiza os billboards das árvores distantes. Pausado no Path Tracing '
                                          'enquanto PT: Billboards estiver ligado. Ligado pode aumentar o FPS. Ray '
                                          'Tracing normal e Path Tracing. Aplica-se às áreas recém-carregadas.',
        'tooltip_cr01_cpu_fast_bone_reads': 'Acelera a leitura pela CPU dos ossos de animação de personagens e '
                                            'criaturas; a animação continua igual. Ligado pode aumentar o FPS. '
                                            'Aplica-se imediatamente.',
        'tooltip_cr01_cpu_physics_lod': 'Reduz o trabalho da CPU com detalhes de física; objetos distantes e tecidos '
                                        'podem se mover com menos detalhe. Ligado pode aumentar o FPS. Aplica-se '
                                        'imediatamente.',
        'tooltip_cr01_geometrybounces': 'Quantas vezes a luz rebate nas superfícies. Valores menores podem aumentar o '
                                        'FPS. Apenas Path Tracing. Aplica-se imediatamente.',
        'tooltip_cr01_grassday': 'Brilho da grama de dia. Preferência visual; sem ganho de FPS comprovado. Apenas Path '
                                 'Tracing. Aplica-se imediatamente.',
        'tooltip_cr01_grassnight': 'Brilho da grama à noite. Preferência visual; sem ganho de FPS comprovado. Apenas '
                                   'Path Tracing. Aplica-se imediatamente.',
        'tooltip_cr01_hairbounces': 'Quantas vezes a luz rebate dentro do cabelo. Valores menores podem aumentar o '
                                    'FPS. Apenas Path Tracing. Aplica-se imediatamente.',
        'tooltip_cr01_indirectlight': 'Brilho da luz indireta (1.00 = neutro). Preferência visual; sem ganho de FPS '
                                      'comprovado. Apenas Path Tracing. Aplica-se imediatamente.',
        'tooltip_cr01_pt_scene_range': 'Reduz a área com ray tracing ao seu redor (jogo: 400 m); reflexos, luz '
                                       'indireta e sombras distantes podem diminuir. Valores menores podem aumentar o '
                                       'FPS. Apenas Path Tracing. Aplica-se imediatamente.',
        'tooltip_cr01_ptbillboards': 'Sombras dos billboards de árvores distantes. Ligado pausa CPU: Distant Trees. '
                                     'Desligado pode aumentar o FPS. Apenas Path Tracing. Aplica-se às áreas '
                                     'recém-carregadas.',
        'tooltip_cr01_ptshadows': 'Sombras com ray tracing; Desligado perde detalhes de sombra. Desligado pode '
                                  'aumentar o FPS. Apenas Path Tracing. Aplica-se imediatamente.',
        'tooltip_cr01_rt_perf_ao': 'Qualidade do pré-traçado das sombras de contato. Valores menores podem aumentar o '
                                   'FPS. Apenas Ray Tracing normal. Aplica-se imediatamente. Exige Ray-Traced '
                                   'Optimizations em Auto.',
        'tooltip_cr01_rt_perf_budget': 'Trabalho de iluminação por quadro; orçamentos baixos podem atrasar a luz ou '
                                       'desbotar a grama. Valores menores podem aumentar o FPS. Apenas Ray Tracing '
                                       'normal. Aplica-se imediatamente. Exige Ray-Traced Optimizations em Auto.',
        'tooltip_cr01_rt_perf_cap': 'Limita quantas sondas de luz são classificadas; limites baixos podem atrasar a '
                                    'iluminação. Valores menores podem aumentar o FPS. Apenas Ray Tracing normal. '
                                    'Aplica-se imediatamente. Exige Ray-Traced Optimizations em Auto.',
        'tooltip_cr01_rt_perf_global_shading_limit': 'Sondas da grade global sombreadas por quadro; limites baixos '
                                                     'podem atrasar a iluminação. Valores menores podem aumentar o '
                                                     'FPS. Apenas Ray Tracing normal. Aplica-se imediatamente. Exige '
                                                     'Ray-Traced Optimizations em Auto.',
        'tooltip_cr01_rt_perf_grid1_shading_limit': 'Sondas da grade secundária sombreadas por quadro; limites baixos '
                                                    'podem atrasar a iluminação. Valores menores podem aumentar o FPS. '
                                                    'Apenas Ray Tracing normal. Aplica-se imediatamente. Exige '
                                                    'Ray-Traced Optimizations em Auto.',
        'tooltip_cr01_rt_perf_primary': 'Raios de classificação primários da iluminação global; menos raios podem '
                                        'mudar a iluminação. Valores menores podem aumentar o FPS. Apenas Ray Tracing '
                                        'normal. Aplica-se imediatamente. Exige Ray-Traced Optimizations em Auto.',
        'tooltip_cr01_rt_perf_secondary': 'Raios de classificação secundários da iluminação global; menos raios podem '
                                          'mudar a iluminação. Valores menores podem aumentar o FPS. Apenas Ray '
                                          'Tracing normal. Aplica-se imediatamente. Exige Ray-Traced Optimizations em '
                                          'Auto.',
        'tooltip_cr01_rt_quality_ao': 'Qualidade do pré-traçado das sombras de contato. Valores menores podem aumentar '
                                      'o FPS. Apenas Ray Tracing normal. Aplica-se imediatamente. Exige Ray-Traced '
                                      'Optimizations em Auto.',
        'tooltip_cr01_rt_quality_budget': 'Trabalho de iluminação por quadro; orçamentos baixos podem atrasar a luz ou '
                                          'desbotar a grama. Valores menores podem aumentar o FPS. Apenas Ray Tracing '
                                          'normal. Aplica-se imediatamente. Exige Ray-Traced Optimizations em Auto.',
        'tooltip_cr01_rt_quality_cap': 'Limita quantas sondas de luz são classificadas; limites baixos podem atrasar a '
                                       'iluminação. Valores menores podem aumentar o FPS. Apenas Ray Tracing normal. '
                                       'Aplica-se imediatamente. Exige Ray-Traced Optimizations em Auto.',
        'tooltip_cr01_rt_quality_global_shading_limit': 'Sondas da grade global sombreadas por quadro; limites baixos '
                                                        'podem atrasar a iluminação. Valores menores podem aumentar o '
                                                        'FPS. Apenas Ray Tracing normal. Aplica-se imediatamente. '
                                                        'Exige Ray-Traced Optimizations em Auto.',
        'tooltip_cr01_rt_quality_grid1_shading_limit': 'Sondas da grade secundária sombreadas por quadro; limites '
                                                       'baixos podem atrasar a iluminação. Valores menores podem '
                                                       'aumentar o FPS. Apenas Ray Tracing normal. Aplica-se '
                                                       'imediatamente. Exige Ray-Traced Optimizations em Auto.',
        'tooltip_cr01_rt_quality_primary': 'Raios de classificação primários da iluminação global; menos raios podem '
                                           'mudar a iluminação. Valores menores podem aumentar o FPS. Apenas Ray '
                                           'Tracing normal. Aplica-se imediatamente. Exige Ray-Traced Optimizations em '
                                           'Auto.',
        'tooltip_cr01_rt_quality_secondary': 'Raios de classificação secundários da iluminação global; menos raios '
                                             'podem mudar a iluminação. Valores menores podem aumentar o FPS. Apenas '
                                             'Ray Tracing normal. Aplica-se imediatamente. Exige Ray-Traced '
                                             'Optimizations em Auto.',
        'tooltip_cr01_rt_refl_local_lights': 'Desligado ignora lâmpadas, tochas e fogueiras ao iluminar os reflexos; '
                                             'áreas iluminadas ficam mais escuras neles. Desligado pode aumentar o '
                                             'FPS. Apenas Ray Tracing normal. Aplica-se imediatamente.',
        'tooltip_cr01_rt_shadow_foliage_detail': 'Usa texturas de folhas mais grosseiras para os raios de sombra; as '
                                                 'sombras das folhas ficam mais suaves. Menos detalhe pode aumentar o '
                                                 'FPS. Apenas Ray Tracing normal. Aplica-se imediatamente.',
        'tooltip_cr01_rt_shadow_local_lights': 'Quantas luzes próximas projetam sombras com ray tracing (jogo: 8); '
                                               'sombras do sol não mudam. Valores menores podem aumentar o FPS. Apenas '
                                               'Ray Tracing normal. Aplica-se imediatamente.',
        'tooltip_cr01_skyimpact': 'Força da luz do céu (1.00 = neutro). Preferência visual; sem ganho de FPS '
                                  'comprovado. Apenas Path Tracing. Aplica-se imediatamente.',
        'tooltip_cr01_specularcheckerboard': 'Amostragem em xadrez para alguns reflexos; Ligado pode perder algum '
                                             'detalhe. Ligado pode aumentar o FPS. Apenas Path Tracing. Aplica-se '
                                             'imediatamente.',
        'tooltip_cr01_specularscreenspace': 'Ajuda em espaço de tela para alguns reflexos; Desligado pode perder '
                                            'detalhes. Desligado pode aumentar o FPS. Apenas Path Tracing. Aplica-se '
                                            'imediatamente.',
        'tooltip_cr01_sunimpact': 'Força da luz do sol (1.00 = neutro). Preferência visual; sem ganho de FPS '
                                  'comprovado. Apenas Path Tracing. Aplica-se imediatamente.'},
 'cn': {'tooltip_cr01_ambientocclusion': '游戏自带的环境光遮蔽。关闭可能提升 FPS。仅限 Path Tracing。立即生效。',
        'tooltip_cr01_aocontrast': '环境光遮蔽的对比度。属于视觉偏好，没有已确认的 FPS 收益。仅限 Path Tracing。立即生效。',
        'tooltip_cr01_aoradius': '环境光遮蔽的影响范围。属于视觉偏好，没有已确认的 FPS 收益。仅限 Path Tracing。立即生效。',
        'tooltip_cr01_aostrength': '环境光遮蔽的暗度。属于视觉偏好，没有已确认的 FPS 收益。仅限 Path Tracing。立即生效。',
        'tooltip_cr01_cpu_distant_trees': '优化远处树木的公告板。PT: Billboards 开启时，在 Path Tracing 中暂停。开启可能提升 FPS。普通 Ray Tracing '
                                          '与 Path Tracing。对新加载的区域生效。',
        'tooltip_cr01_cpu_fast_bone_reads': '加快 CPU 读取角色和生物动画骨骼的速度；动画效果不变。开启可能提升 FPS。立即生效。',
        'tooltip_cr01_cpu_physics_lod': '减少物理细节的 CPU 工作；远处物体和布料的运动可能不够精细。开启可能提升 FPS。立即生效。',
        'tooltip_cr01_geometrybounces': '光线在表面之间反弹的次数。降低数值可能提升 FPS。仅限 Path Tracing。立即生效。',
        'tooltip_cr01_grassday': '白天的草地亮度。属于视觉偏好，没有已确认的 FPS 收益。仅限 Path Tracing。立即生效。',
        'tooltip_cr01_grassnight': '夜晚的草地亮度。属于视觉偏好，没有已确认的 FPS 收益。仅限 Path Tracing。立即生效。',
        'tooltip_cr01_hairbounces': '光线在头发内部反弹的次数。降低数值可能提升 FPS。仅限 Path Tracing。立即生效。',
        'tooltip_cr01_indirectlight': '间接光亮度（1.00 = 中性）。属于视觉偏好，没有已确认的 FPS 收益。仅限 Path Tracing。立即生效。',
        'tooltip_cr01_pt_scene_range': '缩小你周围的光线追踪范围（游戏默认：400 m）；远处的反射、间接光和阴影可能减弱。降低数值可能提升 FPS。仅限 Path Tracing。立即生效。',
        'tooltip_cr01_ptbillboards': '远处树木公告板的阴影。开启会暂停 CPU: Distant Trees。关闭可能提升 FPS。仅限 Path Tracing。对新加载的区域生效。',
        'tooltip_cr01_ptshadows': '光线追踪阴影；关闭会损失阴影细节。关闭可能提升 FPS。仅限 Path Tracing。立即生效。',
        'tooltip_cr01_rt_perf_ao': '接触阴影预追踪的质量。降低数值可能提升 FPS。仅限普通 Ray Tracing。立即生效。需要将 Ray-Traced Optimizations 设为 '
                                   'Auto。',
        'tooltip_cr01_rt_perf_budget': '每帧的光照工作量；过低可能延迟光照或使草地变淡。降低数值可能提升 FPS。仅限普通 Ray Tracing。立即生效。需要将 Ray-Traced '
                                       'Optimizations 设为 Auto。',
        'tooltip_cr01_rt_perf_cap': '限制被分类的光照探针数量；过低可能延迟光照更新。降低数值可能提升 FPS。仅限普通 Ray Tracing。立即生效。需要将 Ray-Traced '
                                    'Optimizations 设为 Auto。',
        'tooltip_cr01_rt_perf_global_shading_limit': '全局网格每帧着色的探针数；过低可能延迟光照更新。降低数值可能提升 FPS。仅限普通 Ray Tracing。立即生效。需要将 '
                                                     'Ray-Traced Optimizations 设为 Auto。',
        'tooltip_cr01_rt_perf_grid1_shading_limit': '次级网格每帧着色的探针数；过低可能延迟光照更新。降低数值可能提升 FPS。仅限普通 Ray Tracing。立即生效。需要将 '
                                                    'Ray-Traced Optimizations 设为 Auto。',
        'tooltip_cr01_rt_perf_primary': '全局光照的主分类光线；减少光线可能改变光照。降低数值可能提升 FPS。仅限普通 Ray Tracing。立即生效。需要将 Ray-Traced '
                                        'Optimizations 设为 Auto。',
        'tooltip_cr01_rt_perf_secondary': '全局光照的次分类光线；减少光线可能改变光照。降低数值可能提升 FPS。仅限普通 Ray Tracing。立即生效。需要将 Ray-Traced '
                                          'Optimizations 设为 Auto。',
        'tooltip_cr01_rt_quality_ao': '接触阴影预追踪的质量。降低数值可能提升 FPS。仅限普通 Ray Tracing。立即生效。需要将 Ray-Traced Optimizations 设为 '
                                      'Auto。',
        'tooltip_cr01_rt_quality_budget': '每帧的光照工作量；过低可能延迟光照或使草地变淡。降低数值可能提升 FPS。仅限普通 Ray Tracing。立即生效。需要将 Ray-Traced '
                                          'Optimizations 设为 Auto。',
        'tooltip_cr01_rt_quality_cap': '限制被分类的光照探针数量；过低可能延迟光照更新。降低数值可能提升 FPS。仅限普通 Ray Tracing。立即生效。需要将 Ray-Traced '
                                       'Optimizations 设为 Auto。',
        'tooltip_cr01_rt_quality_global_shading_limit': '全局网格每帧着色的探针数；过低可能延迟光照更新。降低数值可能提升 FPS。仅限普通 Ray '
                                                        'Tracing。立即生效。需要将 Ray-Traced Optimizations 设为 Auto。',
        'tooltip_cr01_rt_quality_grid1_shading_limit': '次级网格每帧着色的探针数；过低可能延迟光照更新。降低数值可能提升 FPS。仅限普通 Ray Tracing。立即生效。需要将 '
                                                       'Ray-Traced Optimizations 设为 Auto。',
        'tooltip_cr01_rt_quality_primary': '全局光照的主分类光线；减少光线可能改变光照。降低数值可能提升 FPS。仅限普通 Ray Tracing。立即生效。需要将 Ray-Traced '
                                           'Optimizations 设为 Auto。',
        'tooltip_cr01_rt_quality_secondary': '全局光照的次分类光线；减少光线可能改变光照。降低数值可能提升 FPS。仅限普通 Ray Tracing。立即生效。需要将 Ray-Traced '
                                             'Optimizations 设为 Auto。',
        'tooltip_cr01_rt_refl_local_lights': '关闭后照亮反射时会忽略灯、火把和火焰；被照亮的区域在反射中更暗。关闭可能提升 FPS。仅限普通 Ray Tracing。立即生效。',
        'tooltip_cr01_rt_shadow_foliage_detail': '阴影光线使用更粗糙的树叶纹理；树叶阴影变得更柔和。降低细节可能提升 FPS。仅限普通 Ray Tracing。立即生效。',
        'tooltip_cr01_rt_shadow_local_lights': '投射光线追踪阴影的附近光源数量（游戏默认：8）；太阳阴影不受影响。降低数值可能提升 FPS。仅限普通 Ray Tracing。立即生效。',
        'tooltip_cr01_skyimpact': '天空光强度（1.00 = 中性）。属于视觉偏好，没有已确认的 FPS 收益。仅限 Path Tracing。立即生效。',
        'tooltip_cr01_specularcheckerboard': '部分反射的棋盘格采样；开启可能损失少量细节。开启可能提升 FPS。仅限 Path Tracing。立即生效。',
        'tooltip_cr01_specularscreenspace': '部分反射的屏幕空间辅助；关闭可能损失反射细节。关闭可能提升 FPS。仅限 Path Tracing。立即生效。',
        'tooltip_cr01_sunimpact': '阳光强度（1.00 = 中性）。属于视觉偏好，没有已确认的 FPS 收益。仅限 Path Tracing。立即生效。'},
 'cz': {'tooltip_cr01_ambientocclusion': 'Okluze okolí ze hry. Vypnuto může zvýšit FPS. Pouze Path Tracing. Projeví se '
                                         'okamžitě.',
        'tooltip_cr01_aocontrast': 'Kontrast okluze okolí. Vzhledová volba; bez prokázaného přínosu pro FPS. Pouze '
                                   'Path Tracing. Projeví se okamžitě.',
        'tooltip_cr01_aoradius': 'Jak daleko okluze okolí sahá. Vzhledová volba; bez prokázaného přínosu pro FPS. '
                                 'Pouze Path Tracing. Projeví se okamžitě.',
        'tooltip_cr01_aostrength': 'Jak tmavá je okluze okolí. Vzhledová volba; bez prokázaného přínosu pro FPS. Pouze '
                                   'Path Tracing. Projeví se okamžitě.',
        'tooltip_cr01_cpu_distant_trees': 'Optimalizuje billboardy vzdálených stromů. V Path Tracingu pozastaveno, '
                                          'dokud je PT: Billboards zapnuto. Zapnuto může zvýšit FPS. Běžný Ray Tracing '
                                          'a Path Tracing. Projeví se v nově načtených oblastech.',
        'tooltip_cr01_cpu_fast_bone_reads': 'Zrychluje čtení animačních kostí postav a tvorů procesorem; animace '
                                            'vypadá stejně. Zapnuto může zvýšit FPS. Projeví se okamžitě.',
        'tooltip_cr01_cpu_physics_lod': 'Snižuje práci CPU na detailech fyziky; vzdálené objekty a látky se mohou '
                                        'pohybovat méně detailně. Zapnuto může zvýšit FPS. Projeví se okamžitě.',
        'tooltip_cr01_geometrybounces': 'Kolikrát se světlo odrazí od povrchů. Nižší hodnoty mohou zvýšit FPS. Pouze '
                                        'Path Tracing. Projeví se okamžitě.',
        'tooltip_cr01_grassday': 'Jas trávy ve dne. Vzhledová volba; bez prokázaného přínosu pro FPS. Pouze Path '
                                 'Tracing. Projeví se okamžitě.',
        'tooltip_cr01_grassnight': 'Jas trávy v noci. Vzhledová volba; bez prokázaného přínosu pro FPS. Pouze Path '
                                   'Tracing. Projeví se okamžitě.',
        'tooltip_cr01_hairbounces': 'Kolikrát se světlo odrazí uvnitř vlasů. Nižší hodnoty mohou zvýšit FPS. Pouze '
                                    'Path Tracing. Projeví se okamžitě.',
        'tooltip_cr01_indirectlight': 'Jas nepřímého světla (1.00 = neutrální). Vzhledová volba; bez prokázaného '
                                      'přínosu pro FPS. Pouze Path Tracing. Projeví se okamžitě.',
        'tooltip_cr01_pt_scene_range': 'Zmenšuje oblast ray tracingu kolem vás (hra: 400 m); vzdálené odrazy, nepřímé '
                                       'světlo a stíny mohou slábnout. Nižší hodnoty mohou zvýšit FPS. Pouze Path '
                                       'Tracing. Projeví se okamžitě.',
        'tooltip_cr01_ptbillboards': 'Stíny billboardů vzdálených stromů. Zapnuto pozastaví CPU: Distant Trees. '
                                     'Vypnuto může zvýšit FPS. Pouze Path Tracing. Projeví se v nově načtených '
                                     'oblastech.',
        'tooltip_cr01_ptshadows': 'Stíny s ray tracingem; Vypnuto ztrácí detaily stínů. Vypnuto může zvýšit FPS. Pouze '
                                  'Path Tracing. Projeví se okamžitě.',
        'tooltip_cr01_rt_perf_ao': 'Kvalita předběžného trasování kontaktních stínů. Nižší hodnoty mohou zvýšit FPS. '
                                   'Pouze běžný Ray Tracing. Projeví se okamžitě. Vyžaduje Ray-Traced Optimizations na '
                                   'Auto.',
        'tooltip_cr01_rt_perf_budget': 'Práce osvětlení na snímek; nízký rozpočet může zpozdit světlo nebo vyblednout '
                                       'trávu. Nižší hodnoty mohou zvýšit FPS. Pouze běžný Ray Tracing. Projeví se '
                                       'okamžitě. Vyžaduje Ray-Traced Optimizations na Auto.',
        'tooltip_cr01_rt_perf_cap': 'Omezuje počet klasifikovaných světelných sond; nízké limity mohou zpozdit '
                                    'osvětlení. Nižší hodnoty mohou zvýšit FPS. Pouze běžný Ray Tracing. Projeví se '
                                    'okamžitě. Vyžaduje Ray-Traced Optimizations na Auto.',
        'tooltip_cr01_rt_perf_global_shading_limit': 'Sondy globální mřížky stínované za snímek; nízké limity mohou '
                                                     'zpozdit osvětlení. Nižší hodnoty mohou zvýšit FPS. Pouze běžný '
                                                     'Ray Tracing. Projeví se okamžitě. Vyžaduje Ray-Traced '
                                                     'Optimizations na Auto.',
        'tooltip_cr01_rt_perf_grid1_shading_limit': 'Sondy sekundární mřížky stínované za snímek; nízké limity mohou '
                                                    'zpozdit osvětlení. Nižší hodnoty mohou zvýšit FPS. Pouze běžný '
                                                    'Ray Tracing. Projeví se okamžitě. Vyžaduje Ray-Traced '
                                                    'Optimizations na Auto.',
        'tooltip_cr01_rt_perf_primary': 'Primární klasifikační paprsky globálního osvětlení; méně paprsků může změnit '
                                        'osvětlení. Nižší hodnoty mohou zvýšit FPS. Pouze běžný Ray Tracing. Projeví '
                                        'se okamžitě. Vyžaduje Ray-Traced Optimizations na Auto.',
        'tooltip_cr01_rt_perf_secondary': 'Sekundární klasifikační paprsky globálního osvětlení; méně paprsků může '
                                          'změnit osvětlení. Nižší hodnoty mohou zvýšit FPS. Pouze běžný Ray Tracing. '
                                          'Projeví se okamžitě. Vyžaduje Ray-Traced Optimizations na Auto.',
        'tooltip_cr01_rt_quality_ao': 'Kvalita předběžného trasování kontaktních stínů. Nižší hodnoty mohou zvýšit '
                                      'FPS. Pouze běžný Ray Tracing. Projeví se okamžitě. Vyžaduje Ray-Traced '
                                      'Optimizations na Auto.',
        'tooltip_cr01_rt_quality_budget': 'Práce osvětlení na snímek; nízký rozpočet může zpozdit světlo nebo '
                                          'vyblednout trávu. Nižší hodnoty mohou zvýšit FPS. Pouze běžný Ray Tracing. '
                                          'Projeví se okamžitě. Vyžaduje Ray-Traced Optimizations na Auto.',
        'tooltip_cr01_rt_quality_cap': 'Omezuje počet klasifikovaných světelných sond; nízké limity mohou zpozdit '
                                       'osvětlení. Nižší hodnoty mohou zvýšit FPS. Pouze běžný Ray Tracing. Projeví se '
                                       'okamžitě. Vyžaduje Ray-Traced Optimizations na Auto.',
        'tooltip_cr01_rt_quality_global_shading_limit': 'Sondy globální mřížky stínované za snímek; nízké limity mohou '
                                                        'zpozdit osvětlení. Nižší hodnoty mohou zvýšit FPS. Pouze '
                                                        'běžný Ray Tracing. Projeví se okamžitě. Vyžaduje Ray-Traced '
                                                        'Optimizations na Auto.',
        'tooltip_cr01_rt_quality_grid1_shading_limit': 'Sondy sekundární mřížky stínované za snímek; nízké limity '
                                                       'mohou zpozdit osvětlení. Nižší hodnoty mohou zvýšit FPS. Pouze '
                                                       'běžný Ray Tracing. Projeví se okamžitě. Vyžaduje Ray-Traced '
                                                       'Optimizations na Auto.',
        'tooltip_cr01_rt_quality_primary': 'Primární klasifikační paprsky globálního osvětlení; méně paprsků může '
                                           'změnit osvětlení. Nižší hodnoty mohou zvýšit FPS. Pouze běžný Ray Tracing. '
                                           'Projeví se okamžitě. Vyžaduje Ray-Traced Optimizations na Auto.',
        'tooltip_cr01_rt_quality_secondary': 'Sekundární klasifikační paprsky globálního osvětlení; méně paprsků může '
                                             'změnit osvětlení. Nižší hodnoty mohou zvýšit FPS. Pouze běžný Ray '
                                             'Tracing. Projeví se okamžitě. Vyžaduje Ray-Traced Optimizations na Auto.',
        'tooltip_cr01_rt_refl_local_lights': 'Vypnuto ignoruje lampy, pochodně a ohně při osvětlení odrazů; osvětlená '
                                             'místa v nich vypadají tmavší. Vypnuto může zvýšit FPS. Pouze běžný Ray '
                                             'Tracing. Projeví se okamžitě.',
        'tooltip_cr01_rt_shadow_foliage_detail': 'Používá hrubší textury listů pro stínové paprsky; stíny listí jsou '
                                                 'měkčí. Méně detailů může zvýšit FPS. Pouze běžný Ray Tracing. '
                                                 'Projeví se okamžitě.',
        'tooltip_cr01_rt_shadow_local_lights': 'Kolik blízkých světel vrhá stíny s ray tracingem (hra: 8); stíny '
                                               'slunce se nemění. Nižší hodnoty mohou zvýšit FPS. Pouze běžný Ray '
                                               'Tracing. Projeví se okamžitě.',
        'tooltip_cr01_skyimpact': 'Síla světla oblohy (1.00 = neutrální). Vzhledová volba; bez prokázaného přínosu pro '
                                  'FPS. Pouze Path Tracing. Projeví se okamžitě.',
        'tooltip_cr01_specularcheckerboard': 'Šachovnicové vzorkování pro některé odrazy; Zapnuto může ztratit trochu '
                                             'detailu. Zapnuto může zvýšit FPS. Pouze Path Tracing. Projeví se '
                                             'okamžitě.',
        'tooltip_cr01_specularscreenspace': 'Pomoc v prostoru obrazovky pro některé odrazy; Vypnuto může ztratit '
                                            'detaily. Vypnuto může zvýšit FPS. Pouze Path Tracing. Projeví se '
                                            'okamžitě.',
        'tooltip_cr01_sunimpact': 'Síla slunečního světla (1.00 = neutrální). Vzhledová volba; bez prokázaného přínosu '
                                  'pro FPS. Pouze Path Tracing. Projeví se okamžitě.'},
 'de': {'tooltip_cr01_ambientocclusion': 'Die Umgebungsverdeckung des Spiels. Aus kann die FPS erhöhen. Nur Path '
                                         'Tracing. Wirkt sofort.',
        'tooltip_cr01_aocontrast': 'Kontrast der Umgebungsverdeckung. Optische Vorliebe; kein nachgewiesener '
                                   'FPS-Vorteil. Nur Path Tracing. Wirkt sofort.',
        'tooltip_cr01_aoradius': 'Wie weit die Umgebungsverdeckung reicht. Optische Vorliebe; kein nachgewiesener '
                                 'FPS-Vorteil. Nur Path Tracing. Wirkt sofort.',
        'tooltip_cr01_aostrength': 'Wie dunkel die Umgebungsverdeckung erscheint. Optische Vorliebe; kein '
                                   'nachgewiesener FPS-Vorteil. Nur Path Tracing. Wirkt sofort.',
        'tooltip_cr01_cpu_distant_trees': 'Optimiert die Billboards entfernter Bäume. Im Path Tracing pausiert, '
                                          'solange PT: Billboards auf An steht. An kann die FPS erhöhen. Normales RT '
                                          'und Path Tracing. Wirkt für neu geladene Gebiete.',
        'tooltip_cr01_cpu_fast_bone_reads': 'Beschleunigt das Lesen der Animationsknochen von Figuren und Kreaturen '
                                            'durch die CPU; die Animation sieht gleich aus. An kann die FPS erhöhen. '
                                            'Wirkt sofort.',
        'tooltip_cr01_cpu_physics_lod': 'Verringert die CPU-Arbeit für Physikdetails; entfernte Objekte und Stoffe '
                                        'bewegen sich eventuell weniger detailliert. An kann die FPS erhöhen. Wirkt '
                                        'sofort.',
        'tooltip_cr01_geometrybounces': 'Wie oft Licht von Oberflächen abprallt. Niedrigere Werte können die FPS '
                                        'erhöhen. Nur Path Tracing. Wirkt sofort.',
        'tooltip_cr01_grassday': 'Grashelligkeit am Tag. Optische Vorliebe; kein nachgewiesener FPS-Vorteil. Nur Path '
                                 'Tracing. Wirkt sofort.',
        'tooltip_cr01_grassnight': 'Grashelligkeit in der Nacht. Optische Vorliebe; kein nachgewiesener FPS-Vorteil. '
                                   'Nur Path Tracing. Wirkt sofort.',
        'tooltip_cr01_hairbounces': 'Wie oft Licht innerhalb der Haare abprallt. Niedrigere Werte können die FPS '
                                    'erhöhen. Nur Path Tracing. Wirkt sofort.',
        'tooltip_cr01_indirectlight': 'Helligkeit des indirekten Lichts (1.00 = neutral). Optische Vorliebe; kein '
                                      'nachgewiesener FPS-Vorteil. Nur Path Tracing. Wirkt sofort.',
        'tooltip_cr01_pt_scene_range': 'Verkleinert den raytracing-Bereich um dich herum (Spiel: 400 m); entfernte '
                                       'Spiegelungen, indirektes Licht und Schatten können abnehmen. Niedrigere Werte '
                                       'können die FPS erhöhen. Nur Path Tracing. Wirkt sofort.',
        'tooltip_cr01_ptbillboards': 'Schatten der Billboards entfernter Bäume. An pausiert CPU: Distant Trees. Aus '
                                     'kann die FPS erhöhen. Nur Path Tracing. Wirkt für neu geladene Gebiete.',
        'tooltip_cr01_ptshadows': 'Raytracing-Schatten; Aus verliert Schattendetails. Aus kann die FPS erhöhen. Nur '
                                  'Path Tracing. Wirkt sofort.',
        'tooltip_cr01_rt_perf_ao': 'Qualität der Kontaktschatten-Vorverfolgung. Niedrigere Werte können die FPS '
                                   'erhöhen. Nur normales RT. Wirkt sofort. Benötigt Ray-Traced Optimizations auf '
                                   'Auto.',
        'tooltip_cr01_rt_perf_budget': 'Beleuchtungsarbeit pro Bild; niedrige Budgets können Licht verzögern oder Gras '
                                       'verblassen lassen. Niedrigere Werte können die FPS erhöhen. Nur normales RT. '
                                       'Wirkt sofort. Benötigt Ray-Traced Optimizations auf Auto.',
        'tooltip_cr01_rt_perf_cap': 'Begrenzt, wie viele Lichtsonden klassifiziert werden; niedrige Grenzen können '
                                    'Lichtaktualisierungen verzögern. Niedrigere Werte können die FPS erhöhen. Nur '
                                    'normales RT. Wirkt sofort. Benötigt Ray-Traced Optimizations auf Auto.',
        'tooltip_cr01_rt_perf_global_shading_limit': 'Pro Bild schattierte Sonden des globalen Gitters; niedrige '
                                                     'Grenzen können Lichtaktualisierungen verzögern. Niedrigere Werte '
                                                     'können die FPS erhöhen. Nur normales RT. Wirkt sofort. Benötigt '
                                                     'Ray-Traced Optimizations auf Auto.',
        'tooltip_cr01_rt_perf_grid1_shading_limit': 'Pro Bild schattierte Sonden des zweiten Gitters; niedrige Grenzen '
                                                    'können Lichtaktualisierungen verzögern. Niedrigere Werte können '
                                                    'die FPS erhöhen. Nur normales RT. Wirkt sofort. Benötigt '
                                                    'Ray-Traced Optimizations auf Auto.',
        'tooltip_cr01_rt_perf_primary': 'Primäre Klassifizierungsstrahlen der globalen Beleuchtung; weniger Strahlen '
                                        'können die Beleuchtung verändern. Niedrigere Werte können die FPS erhöhen. '
                                        'Nur normales RT. Wirkt sofort. Benötigt Ray-Traced Optimizations auf Auto.',
        'tooltip_cr01_rt_perf_secondary': 'Sekundäre Klassifizierungsstrahlen der globalen Beleuchtung; weniger '
                                          'Strahlen können die Beleuchtung verändern. Niedrigere Werte können die FPS '
                                          'erhöhen. Nur normales RT. Wirkt sofort. Benötigt Ray-Traced Optimizations '
                                          'auf Auto.',
        'tooltip_cr01_rt_quality_ao': 'Qualität der Kontaktschatten-Vorverfolgung. Niedrigere Werte können die FPS '
                                      'erhöhen. Nur normales RT. Wirkt sofort. Benötigt Ray-Traced Optimizations auf '
                                      'Auto.',
        'tooltip_cr01_rt_quality_budget': 'Beleuchtungsarbeit pro Bild; niedrige Budgets können Licht verzögern oder '
                                          'Gras verblassen lassen. Niedrigere Werte können die FPS erhöhen. Nur '
                                          'normales RT. Wirkt sofort. Benötigt Ray-Traced Optimizations auf Auto.',
        'tooltip_cr01_rt_quality_cap': 'Begrenzt, wie viele Lichtsonden klassifiziert werden; niedrige Grenzen können '
                                       'Lichtaktualisierungen verzögern. Niedrigere Werte können die FPS erhöhen. Nur '
                                       'normales RT. Wirkt sofort. Benötigt Ray-Traced Optimizations auf Auto.',
        'tooltip_cr01_rt_quality_global_shading_limit': 'Pro Bild schattierte Sonden des globalen Gitters; niedrige '
                                                        'Grenzen können Lichtaktualisierungen verzögern. Niedrigere '
                                                        'Werte können die FPS erhöhen. Nur normales RT. Wirkt sofort. '
                                                        'Benötigt Ray-Traced Optimizations auf Auto.',
        'tooltip_cr01_rt_quality_grid1_shading_limit': 'Pro Bild schattierte Sonden des zweiten Gitters; niedrige '
                                                       'Grenzen können Lichtaktualisierungen verzögern. Niedrigere '
                                                       'Werte können die FPS erhöhen. Nur normales RT. Wirkt sofort. '
                                                       'Benötigt Ray-Traced Optimizations auf Auto.',
        'tooltip_cr01_rt_quality_primary': 'Primäre Klassifizierungsstrahlen der globalen Beleuchtung; weniger '
                                           'Strahlen können die Beleuchtung verändern. Niedrigere Werte können die FPS '
                                           'erhöhen. Nur normales RT. Wirkt sofort. Benötigt Ray-Traced Optimizations '
                                           'auf Auto.',
        'tooltip_cr01_rt_quality_secondary': 'Sekundäre Klassifizierungsstrahlen der globalen Beleuchtung; weniger '
                                             'Strahlen können die Beleuchtung verändern. Niedrigere Werte können die '
                                             'FPS erhöhen. Nur normales RT. Wirkt sofort. Benötigt Ray-Traced '
                                             'Optimizations auf Auto.',
        'tooltip_cr01_rt_refl_local_lights': 'Aus ignoriert Lampen, Fackeln und Feuer bei der Beleuchtung von '
                                             'Spiegelungen; beleuchtete Bereiche wirken darin dunkler. Aus kann die '
                                             'FPS erhöhen. Nur normales RT. Wirkt sofort.',
        'tooltip_cr01_rt_shadow_foliage_detail': 'Nutzt gröbere Blatttexturen für Schattenstrahlen; Blattschatten '
                                                 'werden weicher. Weniger Detail kann die FPS erhöhen. Nur normales '
                                                 'RT. Wirkt sofort.',
        'tooltip_cr01_rt_shadow_local_lights': 'Wie viele nahe Lichter raytracing-Schatten werfen (Spiel: 8); '
                                               'Sonnenschatten bleiben unverändert. Niedrigere Werte können die FPS '
                                               'erhöhen. Nur normales RT. Wirkt sofort.',
        'tooltip_cr01_skyimpact': 'Stärke des Himmelslichts (1.00 = neutral). Optische Vorliebe; kein nachgewiesener '
                                  'FPS-Vorteil. Nur Path Tracing. Wirkt sofort.',
        'tooltip_cr01_specularcheckerboard': 'Schachbrett-Abtastung für einige Spiegelungen; An kann etwas Detail '
                                             'kosten. An kann die FPS erhöhen. Nur Path Tracing. Wirkt sofort.',
        'tooltip_cr01_specularscreenspace': 'Bildschirmraum-Hilfe für einige Spiegelungen; Aus kann Spiegelungsdetails '
                                            'kosten. Aus kann die FPS erhöhen. Nur Path Tracing. Wirkt sofort.',
        'tooltip_cr01_sunimpact': 'Stärke des Sonnenlichts (1.00 = neutral). Optische Vorliebe; kein nachgewiesener '
                                  'FPS-Vorteil. Nur Path Tracing. Wirkt sofort.'},
 'en': {'tooltip_cr01_ambientocclusion': "The game's ambient contact shading. Off may improve FPS. Path Tracing only. "
                                         'Applies immediately.',
        'tooltip_cr01_aocontrast': 'Contrast of ambient occlusion. Visual preference; no established FPS benefit. Path '
                                   'Tracing only. Applies immediately.',
        'tooltip_cr01_aoradius': 'How far ambient occlusion reaches. Visual preference; no established FPS benefit. '
                                 'Path Tracing only. Applies immediately.',
        'tooltip_cr01_aostrength': 'How dark ambient occlusion appears. Visual preference; no established FPS benefit. '
                                   'Path Tracing only. Applies immediately.',
        'tooltip_cr01_cpu_distant_trees': 'Optimizes distant tree billboards. Paused in Path Tracing while PT: '
                                          'Billboards is On. On may improve FPS. Normal RT and Path Tracing. Applies '
                                          'to newly loaded areas.',
        'tooltip_cr01_cpu_fast_bone_reads': 'Speeds up CPU reads of character and creature animation bones; animation '
                                            'looks the same. On may improve FPS. Applies immediately.',
        'tooltip_cr01_cpu_physics_lod': 'Reduces CPU work on physics detail; distant objects and cloth may move with '
                                        'less detail. On may improve FPS. Applies immediately.',
        'tooltip_cr01_geometrybounces': 'How many times light bounces off surfaces. Lower values may improve FPS. Path '
                                        'Tracing only. Applies immediately.',
        'tooltip_cr01_grassday': 'Grass brightness by day. Visual preference; no established FPS benefit. Path Tracing '
                                 'only. Applies immediately.',
        'tooltip_cr01_grassnight': 'Grass brightness at night. Visual preference; no established FPS benefit. Path '
                                   'Tracing only. Applies immediately.',
        'tooltip_cr01_hairbounces': 'How many times light bounces inside hair. Lower values may improve FPS. Path '
                                    'Tracing only. Applies immediately.',
        'tooltip_cr01_indirectlight': 'Brightness of bounced light (1.00 = neutral). Visual preference; no established '
                                      'FPS benefit. Path Tracing only. Applies immediately.',
        'tooltip_cr01_pt_scene_range': 'Shrinks the ray-traced area around you (game: 400 m); distant reflections, '
                                       'bounced light and shadows may be reduced. Lower values may improve FPS. Path '
                                       'Tracing only. Applies immediately.',
        'tooltip_cr01_ptbillboards': 'Shadows from distant tree billboards. On pauses CPU: Distant Trees. Off may '
                                     'improve FPS. Path Tracing only. Applies to newly loaded areas.',
        'tooltip_cr01_ptshadows': 'Ray-traced shadows; Off loses shadow detail. Off may improve FPS. Path Tracing '
                                  'only. Applies immediately.',
        'tooltip_cr01_rt_perf_ao': 'Quality of the contact-shadow pre-trace. Lower values may improve FPS. Normal RT '
                                   'only. Applies immediately. Needs Ray-Traced Optimizations set to Auto.',
        'tooltip_cr01_rt_perf_budget': 'Lighting work per frame; low budgets can delay lighting or fade grass. Lower '
                                       'values may improve FPS. Normal RT only. Applies immediately. Needs Ray-Traced '
                                       'Optimizations set to Auto.',
        'tooltip_cr01_rt_perf_cap': 'Limits how many lighting probes are classified; low limits may delay lighting '
                                    'updates. Lower values may improve FPS. Normal RT only. Applies immediately. Needs '
                                    'Ray-Traced Optimizations set to Auto.',
        'tooltip_cr01_rt_perf_global_shading_limit': 'Global-grid probes shaded per frame; low limits may delay '
                                                     'lighting updates. Lower values may improve FPS. Normal RT only. '
                                                     'Applies immediately. Needs Ray-Traced Optimizations set to Auto.',
        'tooltip_cr01_rt_perf_grid1_shading_limit': 'Secondary-grid probes shaded per frame; low limits may delay '
                                                    'lighting updates. Lower values may improve FPS. Normal RT only. '
                                                    'Applies immediately. Needs Ray-Traced Optimizations set to Auto.',
        'tooltip_cr01_rt_perf_primary': 'Primary classification rays for global illumination; fewer rays may change '
                                        'lighting. Lower values may improve FPS. Normal RT only. Applies immediately. '
                                        'Needs Ray-Traced Optimizations set to Auto.',
        'tooltip_cr01_rt_perf_secondary': 'Secondary classification rays for global illumination; fewer rays may '
                                          'change lighting. Lower values may improve FPS. Normal RT only. Applies '
                                          'immediately. Needs Ray-Traced Optimizations set to Auto.',
        'tooltip_cr01_rt_quality_ao': 'Quality of the contact-shadow pre-trace. Lower values may improve FPS. Normal '
                                      'RT only. Applies immediately. Needs Ray-Traced Optimizations set to Auto.',
        'tooltip_cr01_rt_quality_budget': 'Lighting work per frame; low budgets can delay lighting or fade grass. '
                                          'Lower values may improve FPS. Normal RT only. Applies immediately. Needs '
                                          'Ray-Traced Optimizations set to Auto.',
        'tooltip_cr01_rt_quality_cap': 'Limits how many lighting probes are classified; low limits may delay lighting '
                                       'updates. Lower values may improve FPS. Normal RT only. Applies immediately. '
                                       'Needs Ray-Traced Optimizations set to Auto.',
        'tooltip_cr01_rt_quality_global_shading_limit': 'Global-grid probes shaded per frame; low limits may delay '
                                                        'lighting updates. Lower values may improve FPS. Normal RT '
                                                        'only. Applies immediately. Needs Ray-Traced Optimizations set '
                                                        'to Auto.',
        'tooltip_cr01_rt_quality_grid1_shading_limit': 'Secondary-grid probes shaded per frame; low limits may delay '
                                                       'lighting updates. Lower values may improve FPS. Normal RT '
                                                       'only. Applies immediately. Needs Ray-Traced Optimizations set '
                                                       'to Auto.',
        'tooltip_cr01_rt_quality_primary': 'Primary classification rays for global illumination; fewer rays may change '
                                           'lighting. Lower values may improve FPS. Normal RT only. Applies '
                                           'immediately. Needs Ray-Traced Optimizations set to Auto.',
        'tooltip_cr01_rt_quality_secondary': 'Secondary classification rays for global illumination; fewer rays may '
                                             'change lighting. Lower values may improve FPS. Normal RT only. Applies '
                                             'immediately. Needs Ray-Traced Optimizations set to Auto.',
        'tooltip_cr01_rt_refl_local_lights': 'Off ignores lamps, torches and fires when lighting reflections; lit '
                                             'areas look darker in reflections. Off may improve FPS. Normal RT only. '
                                             'Applies immediately.',
        'tooltip_cr01_rt_shadow_foliage_detail': 'Uses coarser leaf textures for shadow rays; leaf shadows become '
                                                 'softer. Lower detail may improve FPS. Normal RT only. Applies '
                                                 'immediately.',
        'tooltip_cr01_rt_shadow_local_lights': 'How many nearby lights cast ray-traced shadows (game: 8); sun shadows '
                                               'are unaffected. Lower values may improve FPS. Normal RT only. Applies '
                                               'immediately.',
        'tooltip_cr01_skyimpact': 'Strength of sky light (1.00 = neutral). Visual preference; no established FPS '
                                  'benefit. Path Tracing only. Applies immediately.',
        'tooltip_cr01_specularcheckerboard': 'Checkerboard sampling for some reflections; On may lose some detail. On '
                                             'may improve FPS. Path Tracing only. Applies immediately.',
        'tooltip_cr01_specularscreenspace': 'Screen-space help for some reflections; Off may lose reflection detail. '
                                            'Off may improve FPS. Path Tracing only. Applies immediately.',
        'tooltip_cr01_sunimpact': 'Strength of sunlight (1.00 = neutral). Visual preference; no established FPS '
                                  'benefit. Path Tracing only. Applies immediately.'},
 'es': {'tooltip_cr01_ambientocclusion': 'La oclusión ambiental del juego. Desactivado puede mejorar los FPS. Solo '
                                         'Path Tracing. Se aplica de inmediato.',
        'tooltip_cr01_aocontrast': 'Contraste de la oclusión ambiental. Preferencia visual; sin beneficio de FPS '
                                   'demostrado. Solo Path Tracing. Se aplica de inmediato.',
        'tooltip_cr01_aoradius': 'Hasta dónde llega la oclusión ambiental. Preferencia visual; sin beneficio de FPS '
                                 'demostrado. Solo Path Tracing. Se aplica de inmediato.',
        'tooltip_cr01_aostrength': 'Lo oscura que se ve la oclusión ambiental. Preferencia visual; sin beneficio de '
                                   'FPS demostrado. Solo Path Tracing. Se aplica de inmediato.',
        'tooltip_cr01_cpu_distant_trees': 'Optimiza los billboards de los árboles lejanos. En pausa en Path Tracing '
                                          'mientras PT: Billboards esté activado. Activado puede mejorar los FPS. Ray '
                                          'Tracing normal y Path Tracing. Se aplica a las zonas cargadas de nuevo.',
        'tooltip_cr01_cpu_fast_bone_reads': 'Acelera la lectura por la CPU de los huesos de animación de personajes y '
                                            'criaturas; la animación no cambia. Activado puede mejorar los FPS. Se '
                                            'aplica de inmediato.',
        'tooltip_cr01_cpu_physics_lod': 'Reduce el trabajo de la CPU en detalles físicos; los objetos lejanos y la '
                                        'ropa pueden moverse con menos detalle. Activado puede mejorar los FPS. Se '
                                        'aplica de inmediato.',
        'tooltip_cr01_geometrybounces': 'Cuántas veces rebota la luz en las superficies. Valores más bajos pueden '
                                        'mejorar los FPS. Solo Path Tracing. Se aplica de inmediato.',
        'tooltip_cr01_grassday': 'Brillo de la hierba de día. Preferencia visual; sin beneficio de FPS demostrado. '
                                 'Solo Path Tracing. Se aplica de inmediato.',
        'tooltip_cr01_grassnight': 'Brillo de la hierba de noche. Preferencia visual; sin beneficio de FPS demostrado. '
                                   'Solo Path Tracing. Se aplica de inmediato.',
        'tooltip_cr01_hairbounces': 'Cuántas veces rebota la luz dentro del pelo. Valores más bajos pueden mejorar los '
                                    'FPS. Solo Path Tracing. Se aplica de inmediato.',
        'tooltip_cr01_indirectlight': 'Brillo de la luz indirecta (1.00 = neutro). Preferencia visual; sin beneficio '
                                      'de FPS demostrado. Solo Path Tracing. Se aplica de inmediato.',
        'tooltip_cr01_pt_scene_range': 'Reduce la zona con ray tracing a tu alrededor (juego: 400 m); los reflejos, la '
                                       'luz indirecta y las sombras lejanas pueden disminuir. Valores más bajos pueden '
                                       'mejorar los FPS. Solo Path Tracing. Se aplica de inmediato.',
        'tooltip_cr01_ptbillboards': 'Sombras de los billboards de árboles lejanos. Activado pausa CPU: Distant Trees. '
                                     'Desactivado puede mejorar los FPS. Solo Path Tracing. Se aplica a las zonas '
                                     'cargadas de nuevo.',
        'tooltip_cr01_ptshadows': 'Sombras con ray tracing; Desactivado pierde detalle de sombra. Desactivado puede '
                                  'mejorar los FPS. Solo Path Tracing. Se aplica de inmediato.',
        'tooltip_cr01_rt_perf_ao': 'Calidad del pretrazado de las sombras de contacto. Valores más bajos pueden '
                                   'mejorar los FPS. Solo Ray Tracing normal. Se aplica de inmediato. Requiere '
                                   'Ray-Traced Optimizations en Auto.',
        'tooltip_cr01_rt_perf_budget': 'Trabajo de iluminación por fotograma; presupuestos bajos pueden retrasar la '
                                       'luz o desvanecer la hierba. Valores más bajos pueden mejorar los FPS. Solo Ray '
                                       'Tracing normal. Se aplica de inmediato. Requiere Ray-Traced Optimizations en '
                                       'Auto.',
        'tooltip_cr01_rt_perf_cap': 'Limita cuántas sondas de luz se clasifican; límites bajos pueden retrasar la '
                                    'iluminación. Valores más bajos pueden mejorar los FPS. Solo Ray Tracing normal. '
                                    'Se aplica de inmediato. Requiere Ray-Traced Optimizations en Auto.',
        'tooltip_cr01_rt_perf_global_shading_limit': 'Sondas de la cuadrícula global sombreadas por fotograma; límites '
                                                     'bajos pueden retrasar la iluminación. Valores más bajos pueden '
                                                     'mejorar los FPS. Solo Ray Tracing normal. Se aplica de '
                                                     'inmediato. Requiere Ray-Traced Optimizations en Auto.',
        'tooltip_cr01_rt_perf_grid1_shading_limit': 'Sondas de la cuadrícula secundaria sombreadas por fotograma; '
                                                    'límites bajos pueden retrasar la iluminación. Valores más bajos '
                                                    'pueden mejorar los FPS. Solo Ray Tracing normal. Se aplica de '
                                                    'inmediato. Requiere Ray-Traced Optimizations en Auto.',
        'tooltip_cr01_rt_perf_primary': 'Rayos de clasificación primarios de la iluminación global; menos rayos pueden '
                                        'cambiar la iluminación. Valores más bajos pueden mejorar los FPS. Solo Ray '
                                        'Tracing normal. Se aplica de inmediato. Requiere Ray-Traced Optimizations en '
                                        'Auto.',
        'tooltip_cr01_rt_perf_secondary': 'Rayos de clasificación secundarios de la iluminación global; menos rayos '
                                          'pueden cambiar la iluminación. Valores más bajos pueden mejorar los FPS. '
                                          'Solo Ray Tracing normal. Se aplica de inmediato. Requiere Ray-Traced '
                                          'Optimizations en Auto.',
        'tooltip_cr01_rt_quality_ao': 'Calidad del pretrazado de las sombras de contacto. Valores más bajos pueden '
                                      'mejorar los FPS. Solo Ray Tracing normal. Se aplica de inmediato. Requiere '
                                      'Ray-Traced Optimizations en Auto.',
        'tooltip_cr01_rt_quality_budget': 'Trabajo de iluminación por fotograma; presupuestos bajos pueden retrasar la '
                                          'luz o desvanecer la hierba. Valores más bajos pueden mejorar los FPS. Solo '
                                          'Ray Tracing normal. Se aplica de inmediato. Requiere Ray-Traced '
                                          'Optimizations en Auto.',
        'tooltip_cr01_rt_quality_cap': 'Limita cuántas sondas de luz se clasifican; límites bajos pueden retrasar la '
                                       'iluminación. Valores más bajos pueden mejorar los FPS. Solo Ray Tracing '
                                       'normal. Se aplica de inmediato. Requiere Ray-Traced Optimizations en Auto.',
        'tooltip_cr01_rt_quality_global_shading_limit': 'Sondas de la cuadrícula global sombreadas por fotograma; '
                                                        'límites bajos pueden retrasar la iluminación. Valores más '
                                                        'bajos pueden mejorar los FPS. Solo Ray Tracing normal. Se '
                                                        'aplica de inmediato. Requiere Ray-Traced Optimizations en '
                                                        'Auto.',
        'tooltip_cr01_rt_quality_grid1_shading_limit': 'Sondas de la cuadrícula secundaria sombreadas por fotograma; '
                                                       'límites bajos pueden retrasar la iluminación. Valores más '
                                                       'bajos pueden mejorar los FPS. Solo Ray Tracing normal. Se '
                                                       'aplica de inmediato. Requiere Ray-Traced Optimizations en '
                                                       'Auto.',
        'tooltip_cr01_rt_quality_primary': 'Rayos de clasificación primarios de la iluminación global; menos rayos '
                                           'pueden cambiar la iluminación. Valores más bajos pueden mejorar los FPS. '
                                           'Solo Ray Tracing normal. Se aplica de inmediato. Requiere Ray-Traced '
                                           'Optimizations en Auto.',
        'tooltip_cr01_rt_quality_secondary': 'Rayos de clasificación secundarios de la iluminación global; menos rayos '
                                             'pueden cambiar la iluminación. Valores más bajos pueden mejorar los FPS. '
                                             'Solo Ray Tracing normal. Se aplica de inmediato. Requiere Ray-Traced '
                                             'Optimizations en Auto.',
        'tooltip_cr01_rt_refl_local_lights': 'Desactivado ignora lámparas, antorchas y fuegos al iluminar los '
                                             'reflejos; las zonas iluminadas se ven más oscuras en ellos. Desactivado '
                                             'puede mejorar los FPS. Solo Ray Tracing normal. Se aplica de inmediato.',
        'tooltip_cr01_rt_shadow_foliage_detail': 'Usa texturas de hojas más bastas para los rayos de sombra; las '
                                                 'sombras de las hojas se suavizan. Menos detalle puede mejorar los '
                                                 'FPS. Solo Ray Tracing normal. Se aplica de inmediato.',
        'tooltip_cr01_rt_shadow_local_lights': 'Cuántas luces cercanas proyectan sombras con ray tracing (juego: 8); '
                                               'las sombras del sol no cambian. Valores más bajos pueden mejorar los '
                                               'FPS. Solo Ray Tracing normal. Se aplica de inmediato.',
        'tooltip_cr01_skyimpact': 'Fuerza de la luz del cielo (1.00 = neutro). Preferencia visual; sin beneficio de '
                                  'FPS demostrado. Solo Path Tracing. Se aplica de inmediato.',
        'tooltip_cr01_specularcheckerboard': 'Muestreo en damero para algunos reflejos; Activado puede perder algo de '
                                             'detalle. Activado puede mejorar los FPS. Solo Path Tracing. Se aplica de '
                                             'inmediato.',
        'tooltip_cr01_specularscreenspace': 'Ayuda en espacio de pantalla para algunos reflejos; Desactivado puede '
                                            'perder detalle. Desactivado puede mejorar los FPS. Solo Path Tracing. Se '
                                            'aplica de inmediato.',
        'tooltip_cr01_sunimpact': 'Fuerza de la luz del sol (1.00 = neutro). Preferencia visual; sin beneficio de FPS '
                                  'demostrado. Solo Path Tracing. Se aplica de inmediato.'},
 'esmx': {'tooltip_cr01_ambientocclusion': 'La oclusión ambiental del juego. Desactivado puede mejorar los FPS. Solo '
                                           'Path Tracing. Se aplica de inmediato.',
          'tooltip_cr01_aocontrast': 'Contraste de la oclusión ambiental. Preferencia visual; sin beneficio de FPS '
                                     'demostrado. Solo Path Tracing. Se aplica de inmediato.',
          'tooltip_cr01_aoradius': 'Hasta dónde llega la oclusión ambiental. Preferencia visual; sin beneficio de FPS '
                                   'demostrado. Solo Path Tracing. Se aplica de inmediato.',
          'tooltip_cr01_aostrength': 'Lo oscura que se ve la oclusión ambiental. Preferencia visual; sin beneficio de '
                                     'FPS demostrado. Solo Path Tracing. Se aplica de inmediato.',
          'tooltip_cr01_cpu_distant_trees': 'Optimiza los billboards de los árboles lejanos. En pausa en Path Tracing '
                                            'mientras PT: Billboards esté activado. Activado puede mejorar los FPS. '
                                            'Ray Tracing normal y Path Tracing. Se aplica a las zonas cargadas de '
                                            'nuevo.',
          'tooltip_cr01_cpu_fast_bone_reads': 'Acelera la lectura por la CPU de los huesos de animación de personajes '
                                              'y criaturas; la animación no cambia. Activado puede mejorar los FPS. Se '
                                              'aplica de inmediato.',
          'tooltip_cr01_cpu_physics_lod': 'Reduce el trabajo de la CPU en detalles físicos; los objetos lejanos y la '
                                          'ropa pueden moverse con menos detalle. Activado puede mejorar los FPS. Se '
                                          'aplica de inmediato.',
          'tooltip_cr01_geometrybounces': 'Cuántas veces rebota la luz en las superficies. Valores más bajos pueden '
                                          'mejorar los FPS. Solo Path Tracing. Se aplica de inmediato.',
          'tooltip_cr01_grassday': 'Brillo de la hierba de día. Preferencia visual; sin beneficio de FPS demostrado. '
                                   'Solo Path Tracing. Se aplica de inmediato.',
          'tooltip_cr01_grassnight': 'Brillo de la hierba de noche. Preferencia visual; sin beneficio de FPS '
                                     'demostrado. Solo Path Tracing. Se aplica de inmediato.',
          'tooltip_cr01_hairbounces': 'Cuántas veces rebota la luz dentro del pelo. Valores más bajos pueden mejorar '
                                      'los FPS. Solo Path Tracing. Se aplica de inmediato.',
          'tooltip_cr01_indirectlight': 'Brillo de la luz indirecta (1.00 = neutro). Preferencia visual; sin beneficio '
                                        'de FPS demostrado. Solo Path Tracing. Se aplica de inmediato.',
          'tooltip_cr01_pt_scene_range': 'Reduce la zona con ray tracing a tu alrededor (juego: 400 m); los reflejos, '
                                         'la luz indirecta y las sombras lejanas pueden disminuir. Valores más bajos '
                                         'pueden mejorar los FPS. Solo Path Tracing. Se aplica de inmediato.',
          'tooltip_cr01_ptbillboards': 'Sombras de los billboards de árboles lejanos. Activado pausa CPU: Distant '
                                       'Trees. Desactivado puede mejorar los FPS. Solo Path Tracing. Se aplica a las '
                                       'zonas cargadas de nuevo.',
          'tooltip_cr01_ptshadows': 'Sombras con ray tracing; Desactivado pierde detalle de sombra. Desactivado puede '
                                    'mejorar los FPS. Solo Path Tracing. Se aplica de inmediato.',
          'tooltip_cr01_rt_perf_ao': 'Calidad del pretrazado de las sombras de contacto. Valores más bajos pueden '
                                     'mejorar los FPS. Solo Ray Tracing normal. Se aplica de inmediato. Requiere '
                                     'Ray-Traced Optimizations en Auto.',
          'tooltip_cr01_rt_perf_budget': 'Trabajo de iluminación por fotograma; presupuestos bajos pueden retrasar la '
                                         'luz o desvanecer la hierba. Valores más bajos pueden mejorar los FPS. Solo '
                                         'Ray Tracing normal. Se aplica de inmediato. Requiere Ray-Traced '
                                         'Optimizations en Auto.',
          'tooltip_cr01_rt_perf_cap': 'Limita cuántas sondas de luz se clasifican; límites bajos pueden retrasar la '
                                      'iluminación. Valores más bajos pueden mejorar los FPS. Solo Ray Tracing normal. '
                                      'Se aplica de inmediato. Requiere Ray-Traced Optimizations en Auto.',
          'tooltip_cr01_rt_perf_global_shading_limit': 'Sondas de la cuadrícula global sombreadas por fotograma; '
                                                       'límites bajos pueden retrasar la iluminación. Valores más '
                                                       'bajos pueden mejorar los FPS. Solo Ray Tracing normal. Se '
                                                       'aplica de inmediato. Requiere Ray-Traced Optimizations en '
                                                       'Auto.',
          'tooltip_cr01_rt_perf_grid1_shading_limit': 'Sondas de la cuadrícula secundaria sombreadas por fotograma; '
                                                      'límites bajos pueden retrasar la iluminación. Valores más bajos '
                                                      'pueden mejorar los FPS. Solo Ray Tracing normal. Se aplica de '
                                                      'inmediato. Requiere Ray-Traced Optimizations en Auto.',
          'tooltip_cr01_rt_perf_primary': 'Rayos de clasificación primarios de la iluminación global; menos rayos '
                                          'pueden cambiar la iluminación. Valores más bajos pueden mejorar los FPS. '
                                          'Solo Ray Tracing normal. Se aplica de inmediato. Requiere Ray-Traced '
                                          'Optimizations en Auto.',
          'tooltip_cr01_rt_perf_secondary': 'Rayos de clasificación secundarios de la iluminación global; menos rayos '
                                            'pueden cambiar la iluminación. Valores más bajos pueden mejorar los FPS. '
                                            'Solo Ray Tracing normal. Se aplica de inmediato. Requiere Ray-Traced '
                                            'Optimizations en Auto.',
          'tooltip_cr01_rt_quality_ao': 'Calidad del pretrazado de las sombras de contacto. Valores más bajos pueden '
                                        'mejorar los FPS. Solo Ray Tracing normal. Se aplica de inmediato. Requiere '
                                        'Ray-Traced Optimizations en Auto.',
          'tooltip_cr01_rt_quality_budget': 'Trabajo de iluminación por fotograma; presupuestos bajos pueden retrasar '
                                            'la luz o desvanecer la hierba. Valores más bajos pueden mejorar los FPS. '
                                            'Solo Ray Tracing normal. Se aplica de inmediato. Requiere Ray-Traced '
                                            'Optimizations en Auto.',
          'tooltip_cr01_rt_quality_cap': 'Limita cuántas sondas de luz se clasifican; límites bajos pueden retrasar la '
                                         'iluminación. Valores más bajos pueden mejorar los FPS. Solo Ray Tracing '
                                         'normal. Se aplica de inmediato. Requiere Ray-Traced Optimizations en Auto.',
          'tooltip_cr01_rt_quality_global_shading_limit': 'Sondas de la cuadrícula global sombreadas por fotograma; '
                                                          'límites bajos pueden retrasar la iluminación. Valores más '
                                                          'bajos pueden mejorar los FPS. Solo Ray Tracing normal. Se '
                                                          'aplica de inmediato. Requiere Ray-Traced Optimizations en '
                                                          'Auto.',
          'tooltip_cr01_rt_quality_grid1_shading_limit': 'Sondas de la cuadrícula secundaria sombreadas por fotograma; '
                                                         'límites bajos pueden retrasar la iluminación. Valores más '
                                                         'bajos pueden mejorar los FPS. Solo Ray Tracing normal. Se '
                                                         'aplica de inmediato. Requiere Ray-Traced Optimizations en '
                                                         'Auto.',
          'tooltip_cr01_rt_quality_primary': 'Rayos de clasificación primarios de la iluminación global; menos rayos '
                                             'pueden cambiar la iluminación. Valores más bajos pueden mejorar los FPS. '
                                             'Solo Ray Tracing normal. Se aplica de inmediato. Requiere Ray-Traced '
                                             'Optimizations en Auto.',
          'tooltip_cr01_rt_quality_secondary': 'Rayos de clasificación secundarios de la iluminación global; menos '
                                               'rayos pueden cambiar la iluminación. Valores más bajos pueden mejorar '
                                               'los FPS. Solo Ray Tracing normal. Se aplica de inmediato. Requiere '
                                               'Ray-Traced Optimizations en Auto.',
          'tooltip_cr01_rt_refl_local_lights': 'Desactivado ignora lámparas, antorchas y fuegos al iluminar los '
                                               'reflejos; las zonas iluminadas se ven más oscuras en ellos. '
                                               'Desactivado puede mejorar los FPS. Solo Ray Tracing normal. Se aplica '
                                               'de inmediato.',
          'tooltip_cr01_rt_shadow_foliage_detail': 'Usa texturas de hojas más bastas para los rayos de sombra; las '
                                                   'sombras de las hojas se suavizan. Menos detalle puede mejorar los '
                                                   'FPS. Solo Ray Tracing normal. Se aplica de inmediato.',
          'tooltip_cr01_rt_shadow_local_lights': 'Cuántas luces cercanas proyectan sombras con ray tracing (juego: 8); '
                                                 'las sombras del sol no cambian. Valores más bajos pueden mejorar los '
                                                 'FPS. Solo Ray Tracing normal. Se aplica de inmediato.',
          'tooltip_cr01_skyimpact': 'Fuerza de la luz del cielo (1.00 = neutro). Preferencia visual; sin beneficio de '
                                    'FPS demostrado. Solo Path Tracing. Se aplica de inmediato.',
          'tooltip_cr01_specularcheckerboard': 'Muestreo en damero para algunos reflejos; Activado puede perder algo '
                                               'de detalle. Activado puede mejorar los FPS. Solo Path Tracing. Se '
                                               'aplica de inmediato.',
          'tooltip_cr01_specularscreenspace': 'Ayuda en espacio de pantalla para algunos reflejos; Desactivado puede '
                                              'perder detalle. Desactivado puede mejorar los FPS. Solo Path Tracing. '
                                              'Se aplica de inmediato.',
          'tooltip_cr01_sunimpact': 'Fuerza de la luz del sol (1.00 = neutro). Preferencia visual; sin beneficio de '
                                    'FPS demostrado. Solo Path Tracing. Se aplica de inmediato.'},
 'fr': {'tooltip_cr01_ambientocclusion': "L'occlusion ambiante du jeu. Désactivé peut augmenter les FPS. Path Tracing "
                                         "uniquement. S'applique immédiatement.",
        'tooltip_cr01_aocontrast': "Contraste de l'occlusion ambiante. Préférence visuelle ; aucun gain de FPS établi. "
                                   "Path Tracing uniquement. S'applique immédiatement.",
        'tooltip_cr01_aoradius': "Portée de l'occlusion ambiante. Préférence visuelle ; aucun gain de FPS établi. Path "
                                 "Tracing uniquement. S'applique immédiatement.",
        'tooltip_cr01_aostrength': "Intensité de l'assombrissement de l'occlusion ambiante. Préférence visuelle ; "
                                   "aucun gain de FPS établi. Path Tracing uniquement. S'applique immédiatement.",
        'tooltip_cr01_cpu_distant_trees': 'Optimise les billboards des arbres lointains. En pause en Path Tracing tant '
                                          'que PT: Billboards est activé. Activé peut augmenter les FPS. Ray Tracing '
                                          "normal et Path Tracing. S'applique aux zones nouvellement chargées.",
        'tooltip_cr01_cpu_fast_bone_reads': "Accélère la lecture par le CPU des os d'animation des personnages et "
                                            "créatures ; l'animation reste identique. Activé peut augmenter les FPS. "
                                            "S'applique immédiatement.",
        'tooltip_cr01_cpu_physics_lod': 'Réduit le travail CPU sur les détails physiques ; les objets lointains et les '
                                        'tissus peuvent bouger avec moins de détail. Activé peut augmenter les FPS. '
                                        "S'applique immédiatement.",
        'tooltip_cr01_geometrybounces': 'Nombre de rebonds de la lumière sur les surfaces. Des valeurs plus basses '
                                        "peuvent augmenter les FPS. Path Tracing uniquement. S'applique immédiatement.",
        'tooltip_cr01_grassday': "Luminosité de l'herbe le jour. Préférence visuelle ; aucun gain de FPS établi. Path "
                                 "Tracing uniquement. S'applique immédiatement.",
        'tooltip_cr01_grassnight': "Luminosité de l'herbe la nuit. Préférence visuelle ; aucun gain de FPS établi. "
                                   "Path Tracing uniquement. S'applique immédiatement.",
        'tooltip_cr01_hairbounces': 'Nombre de rebonds de la lumière dans les cheveux. Des valeurs plus basses peuvent '
                                    "augmenter les FPS. Path Tracing uniquement. S'applique immédiatement.",
        'tooltip_cr01_indirectlight': 'Luminosité de la lumière indirecte (1.00 = neutre). Préférence visuelle ; aucun '
                                      "gain de FPS établi. Path Tracing uniquement. S'applique immédiatement.",
        'tooltip_cr01_pt_scene_range': 'Réduit la zone en ray tracing autour de vous (jeu : 400 m) ; les réflexions, '
                                       'la lumière indirecte et les ombres lointaines peuvent diminuer. Des valeurs '
                                       "plus basses peuvent augmenter les FPS. Path Tracing uniquement. S'applique "
                                       'immédiatement.',
        'tooltip_cr01_ptbillboards': 'Ombres des billboards des arbres lointains. Activé met en pause CPU: Distant '
                                     "Trees. Désactivé peut augmenter les FPS. Path Tracing uniquement. S'applique aux "
                                     'zones nouvellement chargées.',
        'tooltip_cr01_ptshadows': "Ombres en ray tracing ; Désactivé perd des détails d'ombre. Désactivé peut "
                                  "augmenter les FPS. Path Tracing uniquement. S'applique immédiatement.",
        'tooltip_cr01_rt_perf_ao': 'Qualité du pré-traçage des ombres de contact. Des valeurs plus basses peuvent '
                                   "augmenter les FPS. Ray Tracing normal uniquement. S'applique immédiatement. "
                                   'Nécessite Ray-Traced Optimizations sur Auto.',
        'tooltip_cr01_rt_perf_budget': "Travail d'éclairage par image ; un budget bas peut retarder l'éclairage ou "
                                       "faire pâlir l'herbe. Des valeurs plus basses peuvent augmenter les FPS. Ray "
                                       "Tracing normal uniquement. S'applique immédiatement. Nécessite Ray-Traced "
                                       'Optimizations sur Auto.',
        'tooltip_cr01_rt_perf_cap': 'Limite le nombre de sondes de lumière classées ; des limites basses peuvent '
                                    "retarder les mises à jour de l'éclairage. Des valeurs plus basses peuvent "
                                    "augmenter les FPS. Ray Tracing normal uniquement. S'applique immédiatement. "
                                    'Nécessite Ray-Traced Optimizations sur Auto.',
        'tooltip_cr01_rt_perf_global_shading_limit': 'Sondes de la grille globale traitées par image ; des limites '
                                                     "basses peuvent retarder l'éclairage. Des valeurs plus basses "
                                                     'peuvent augmenter les FPS. Ray Tracing normal uniquement. '
                                                     "S'applique immédiatement. Nécessite Ray-Traced Optimizations sur "
                                                     'Auto.',
        'tooltip_cr01_rt_perf_grid1_shading_limit': 'Sondes de la grille secondaire traitées par image ; des limites '
                                                    "basses peuvent retarder l'éclairage. Des valeurs plus basses "
                                                    'peuvent augmenter les FPS. Ray Tracing normal uniquement. '
                                                    "S'applique immédiatement. Nécessite Ray-Traced Optimizations sur "
                                                    'Auto.',
        'tooltip_cr01_rt_perf_primary': "Rayons de classification primaires de l'illumination globale ; moins de "
                                        "rayons peut modifier l'éclairage. Des valeurs plus basses peuvent augmenter "
                                        "les FPS. Ray Tracing normal uniquement. S'applique immédiatement. Nécessite "
                                        'Ray-Traced Optimizations sur Auto.',
        'tooltip_cr01_rt_perf_secondary': "Rayons de classification secondaires de l'illumination globale ; moins de "
                                          "rayons peut modifier l'éclairage. Des valeurs plus basses peuvent augmenter "
                                          "les FPS. Ray Tracing normal uniquement. S'applique immédiatement. Nécessite "
                                          'Ray-Traced Optimizations sur Auto.',
        'tooltip_cr01_rt_quality_ao': 'Qualité du pré-traçage des ombres de contact. Des valeurs plus basses peuvent '
                                      "augmenter les FPS. Ray Tracing normal uniquement. S'applique immédiatement. "
                                      'Nécessite Ray-Traced Optimizations sur Auto.',
        'tooltip_cr01_rt_quality_budget': "Travail d'éclairage par image ; un budget bas peut retarder l'éclairage ou "
                                          "faire pâlir l'herbe. Des valeurs plus basses peuvent augmenter les FPS. Ray "
                                          "Tracing normal uniquement. S'applique immédiatement. Nécessite Ray-Traced "
                                          'Optimizations sur Auto.',
        'tooltip_cr01_rt_quality_cap': 'Limite le nombre de sondes de lumière classées ; des limites basses peuvent '
                                       "retarder les mises à jour de l'éclairage. Des valeurs plus basses peuvent "
                                       "augmenter les FPS. Ray Tracing normal uniquement. S'applique immédiatement. "
                                       'Nécessite Ray-Traced Optimizations sur Auto.',
        'tooltip_cr01_rt_quality_global_shading_limit': 'Sondes de la grille globale traitées par image ; des limites '
                                                        "basses peuvent retarder l'éclairage. Des valeurs plus basses "
                                                        'peuvent augmenter les FPS. Ray Tracing normal uniquement. '
                                                        "S'applique immédiatement. Nécessite Ray-Traced Optimizations "
                                                        'sur Auto.',
        'tooltip_cr01_rt_quality_grid1_shading_limit': 'Sondes de la grille secondaire traitées par image ; des '
                                                       "limites basses peuvent retarder l'éclairage. Des valeurs plus "
                                                       'basses peuvent augmenter les FPS. Ray Tracing normal '
                                                       "uniquement. S'applique immédiatement. Nécessite Ray-Traced "
                                                       'Optimizations sur Auto.',
        'tooltip_cr01_rt_quality_primary': "Rayons de classification primaires de l'illumination globale ; moins de "
                                           "rayons peut modifier l'éclairage. Des valeurs plus basses peuvent "
                                           "augmenter les FPS. Ray Tracing normal uniquement. S'applique "
                                           'immédiatement. Nécessite Ray-Traced Optimizations sur Auto.',
        'tooltip_cr01_rt_quality_secondary': "Rayons de classification secondaires de l'illumination globale ; moins "
                                             "de rayons peut modifier l'éclairage. Des valeurs plus basses peuvent "
                                             "augmenter les FPS. Ray Tracing normal uniquement. S'applique "
                                             'immédiatement. Nécessite Ray-Traced Optimizations sur Auto.',
        'tooltip_cr01_rt_refl_local_lights': 'Désactivé ignore lampes, torches et feux pour éclairer les réflexions ; '
                                             'les zones éclairées y paraissent plus sombres. Désactivé peut augmenter '
                                             "les FPS. Ray Tracing normal uniquement. S'applique immédiatement.",
        'tooltip_cr01_rt_shadow_foliage_detail': 'Utilise des textures de feuilles plus grossières pour les rayons '
                                                 "d'ombre ; les ombres des feuilles deviennent plus douces. Moins de "
                                                 'détail peut augmenter les FPS. Ray Tracing normal uniquement. '
                                                 "S'applique immédiatement.",
        'tooltip_cr01_rt_shadow_local_lights': 'Nombre de lumières proches qui projettent des ombres en ray tracing '
                                               '(jeu : 8) ; les ombres du soleil ne changent pas. Des valeurs plus '
                                               'basses peuvent augmenter les FPS. Ray Tracing normal uniquement. '
                                               "S'applique immédiatement.",
        'tooltip_cr01_skyimpact': 'Force de la lumière du ciel (1.00 = neutre). Préférence visuelle ; aucun gain de '
                                  "FPS établi. Path Tracing uniquement. S'applique immédiatement.",
        'tooltip_cr01_specularcheckerboard': 'Échantillonnage en damier pour certaines réflexions ; Activé peut perdre '
                                             'un peu de détail. Activé peut augmenter les FPS. Path Tracing '
                                             "uniquement. S'applique immédiatement.",
        'tooltip_cr01_specularscreenspace': 'Aide en espace écran pour certaines réflexions ; Désactivé peut perdre '
                                            'des détails de réflexion. Désactivé peut augmenter les FPS. Path Tracing '
                                            "uniquement. S'applique immédiatement.",
        'tooltip_cr01_sunimpact': 'Force de la lumière du soleil (1.00 = neutre). Préférence visuelle ; aucun gain de '
                                  "FPS établi. Path Tracing uniquement. S'applique immédiatement."},
 'hu': {'tooltip_cr01_ambientocclusion': 'A játék környezeti takarása. A Ki növelheti az FPS-t. Csak Path Tracing. '
                                         'Azonnal érvényes.',
        'tooltip_cr01_aocontrast': 'A környezeti takarás kontrasztja. Ízlés kérdése; igazolt FPS-előny nélkül. Csak '
                                   'Path Tracing. Azonnal érvényes.',
        'tooltip_cr01_aoradius': 'Milyen messzire ér a környezeti takarás. Ízlés kérdése; igazolt FPS-előny nélkül. '
                                 'Csak Path Tracing. Azonnal érvényes.',
        'tooltip_cr01_aostrength': 'Milyen sötét a környezeti takarás. Ízlés kérdése; igazolt FPS-előny nélkül. Csak '
                                   'Path Tracing. Azonnal érvényes.',
        'tooltip_cr01_cpu_distant_trees': 'Optimalizálja a távoli fák billboardjait. Path Tracingben szünetel, amíg a '
                                          'PT: Billboards be van kapcsolva. A Be növelheti az FPS-t. Normál Ray '
                                          'Tracing és Path Tracing. Az újonnan betöltött területeken érvényes.',
        'tooltip_cr01_cpu_fast_bone_reads': 'Gyorsítja a szereplők és lények animációs csontjainak CPU-s olvasását; az '
                                            'animáció ugyanúgy néz ki. A Be növelheti az FPS-t. Azonnal érvényes.',
        'tooltip_cr01_cpu_physics_lod': 'Csökkenti a fizikai részletek CPU-munkáját; a távoli tárgyak és a ruhák '
                                        'kevésbé részletesen mozoghatnak. A Be növelheti az FPS-t. Azonnal érvényes.',
        'tooltip_cr01_geometrybounces': 'Hányszor pattan vissza a fény a felületekről. Alacsonyabb értékek növelhetik '
                                        'az FPS-t. Csak Path Tracing. Azonnal érvényes.',
        'tooltip_cr01_grassday': 'A fű fényereje nappal. Ízlés kérdése; igazolt FPS-előny nélkül. Csak Path Tracing. '
                                 'Azonnal érvényes.',
        'tooltip_cr01_grassnight': 'A fű fényereje éjjel. Ízlés kérdése; igazolt FPS-előny nélkül. Csak Path Tracing. '
                                   'Azonnal érvényes.',
        'tooltip_cr01_hairbounces': 'Hányszor pattan vissza a fény a hajon belül. Alacsonyabb értékek növelhetik az '
                                    'FPS-t. Csak Path Tracing. Azonnal érvényes.',
        'tooltip_cr01_indirectlight': 'A szórt fény fényereje (1.00 = semleges). Ízlés kérdése; igazolt FPS-előny '
                                      'nélkül. Csak Path Tracing. Azonnal érvényes.',
        'tooltip_cr01_pt_scene_range': 'Csökkenti a körülötted lévő ray tracinges területet (játék: 400 m); a távoli '
                                       'tükröződések, szórt fény és árnyékok gyengülhetnek. Alacsonyabb értékek '
                                       'növelhetik az FPS-t. Csak Path Tracing. Azonnal érvényes.',
        'tooltip_cr01_ptbillboards': 'A távoli fák billboardjainak árnyékai. A Be szünetelteti a CPU: Distant Trees '
                                     'opciót. A Ki növelheti az FPS-t. Csak Path Tracing. Az újonnan betöltött '
                                     'területeken érvényes.',
        'tooltip_cr01_ptshadows': 'Ray tracinges árnyékok; a Ki árnyékrészleteket veszít. A Ki növelheti az FPS-t. '
                                  'Csak Path Tracing. Azonnal érvényes.',
        'tooltip_cr01_rt_perf_ao': 'Az érintkezési árnyékok előkövetésének minősége. Alacsonyabb értékek növelhetik az '
                                   'FPS-t. Csak normál Ray Tracing. Azonnal érvényes. A Ray-Traced Optimizations Auto '
                                   'állása szükséges.',
        'tooltip_cr01_rt_perf_budget': 'Megvilágítási munka képkockánként; alacsony keret késleltetheti a fényt vagy '
                                       'halványíthatja a füvet. Alacsonyabb értékek növelhetik az FPS-t. Csak normál '
                                       'Ray Tracing. Azonnal érvényes. A Ray-Traced Optimizations Auto állása '
                                       'szükséges.',
        'tooltip_cr01_rt_perf_cap': 'Korlátozza az osztályozott fényszondák számát; alacsony korlát késleltetheti a '
                                    'fényfrissítést. Alacsonyabb értékek növelhetik az FPS-t. Csak normál Ray Tracing. '
                                    'Azonnal érvényes. A Ray-Traced Optimizations Auto állása szükséges.',
        'tooltip_cr01_rt_perf_global_shading_limit': 'A globális rács képkockánként árnyalt szondái; alacsony korlát '
                                                     'késleltetheti a fényfrissítést. Alacsonyabb értékek növelhetik '
                                                     'az FPS-t. Csak normál Ray Tracing. Azonnal érvényes. A '
                                                     'Ray-Traced Optimizations Auto állása szükséges.',
        'tooltip_cr01_rt_perf_grid1_shading_limit': 'A másodlagos rács képkockánként árnyalt szondái; alacsony korlát '
                                                    'késleltetheti a fényfrissítést. Alacsonyabb értékek növelhetik az '
                                                    'FPS-t. Csak normál Ray Tracing. Azonnal érvényes. A Ray-Traced '
                                                    'Optimizations Auto állása szükséges.',
        'tooltip_cr01_rt_perf_primary': 'A globális megvilágítás elsődleges osztályozó sugarai; kevesebb sugár '
                                        'megváltoztathatja a fényeket. Alacsonyabb értékek növelhetik az FPS-t. Csak '
                                        'normál Ray Tracing. Azonnal érvényes. A Ray-Traced Optimizations Auto állása '
                                        'szükséges.',
        'tooltip_cr01_rt_perf_secondary': 'A globális megvilágítás másodlagos osztályozó sugarai; kevesebb sugár '
                                          'megváltoztathatja a fényeket. Alacsonyabb értékek növelhetik az FPS-t. Csak '
                                          'normál Ray Tracing. Azonnal érvényes. A Ray-Traced Optimizations Auto '
                                          'állása szükséges.',
        'tooltip_cr01_rt_quality_ao': 'Az érintkezési árnyékok előkövetésének minősége. Alacsonyabb értékek növelhetik '
                                      'az FPS-t. Csak normál Ray Tracing. Azonnal érvényes. A Ray-Traced Optimizations '
                                      'Auto állása szükséges.',
        'tooltip_cr01_rt_quality_budget': 'Megvilágítási munka képkockánként; alacsony keret késleltetheti a fényt '
                                          'vagy halványíthatja a füvet. Alacsonyabb értékek növelhetik az FPS-t. Csak '
                                          'normál Ray Tracing. Azonnal érvényes. A Ray-Traced Optimizations Auto '
                                          'állása szükséges.',
        'tooltip_cr01_rt_quality_cap': 'Korlátozza az osztályozott fényszondák számát; alacsony korlát késleltetheti a '
                                       'fényfrissítést. Alacsonyabb értékek növelhetik az FPS-t. Csak normál Ray '
                                       'Tracing. Azonnal érvényes. A Ray-Traced Optimizations Auto állása szükséges.',
        'tooltip_cr01_rt_quality_global_shading_limit': 'A globális rács képkockánként árnyalt szondái; alacsony '
                                                        'korlát késleltetheti a fényfrissítést. Alacsonyabb értékek '
                                                        'növelhetik az FPS-t. Csak normál Ray Tracing. Azonnal '
                                                        'érvényes. A Ray-Traced Optimizations Auto állása szükséges.',
        'tooltip_cr01_rt_quality_grid1_shading_limit': 'A másodlagos rács képkockánként árnyalt szondái; alacsony '
                                                       'korlát késleltetheti a fényfrissítést. Alacsonyabb értékek '
                                                       'növelhetik az FPS-t. Csak normál Ray Tracing. Azonnal '
                                                       'érvényes. A Ray-Traced Optimizations Auto állása szükséges.',
        'tooltip_cr01_rt_quality_primary': 'A globális megvilágítás elsődleges osztályozó sugarai; kevesebb sugár '
                                           'megváltoztathatja a fényeket. Alacsonyabb értékek növelhetik az FPS-t. '
                                           'Csak normál Ray Tracing. Azonnal érvényes. A Ray-Traced Optimizations Auto '
                                           'állása szükséges.',
        'tooltip_cr01_rt_quality_secondary': 'A globális megvilágítás másodlagos osztályozó sugarai; kevesebb sugár '
                                             'megváltoztathatja a fényeket. Alacsonyabb értékek növelhetik az FPS-t. '
                                             'Csak normál Ray Tracing. Azonnal érvényes. A Ray-Traced Optimizations '
                                             'Auto állása szükséges.',
        'tooltip_cr01_rt_refl_local_lights': 'A Ki figyelmen kívül hagyja a lámpákat, fáklyákat és tüzeket a '
                                             'tükröződések megvilágításánál; a megvilágított helyek sötétebbek bennük. '
                                             'A Ki növelheti az FPS-t. Csak normál Ray Tracing. Azonnal érvényes.',
        'tooltip_cr01_rt_shadow_foliage_detail': 'Durvább levéltextúrát használ az árnyéksugarakhoz; a levelek árnyéka '
                                                 'lágyabb lesz. Kevesebb részlet növelheti az FPS-t. Csak normál Ray '
                                                 'Tracing. Azonnal érvényes.',
        'tooltip_cr01_rt_shadow_local_lights': 'Hány közeli fény vet ray tracinges árnyékot (játék: 8); a nap árnyékai '
                                               'nem változnak. Alacsonyabb értékek növelhetik az FPS-t. Csak normál '
                                               'Ray Tracing. Azonnal érvényes.',
        'tooltip_cr01_skyimpact': 'Az égbolt fényének erőssége (1.00 = semleges). Ízlés kérdése; igazolt FPS-előny '
                                  'nélkül. Csak Path Tracing. Azonnal érvényes.',
        'tooltip_cr01_specularcheckerboard': 'Sakktábla-mintavételezés egyes tükröződésekhez; a Be némi részletet '
                                             'veszíthet. A Be növelheti az FPS-t. Csak Path Tracing. Azonnal érvényes.',
        'tooltip_cr01_specularscreenspace': 'Képtér-alapú segítség egyes tükröződésekhez; a Ki részleteket veszíthet. '
                                            'A Ki növelheti az FPS-t. Csak Path Tracing. Azonnal érvényes.',
        'tooltip_cr01_sunimpact': 'A napfény erőssége (1.00 = semleges). Ízlés kérdése; igazolt FPS-előny nélkül. Csak '
                                  'Path Tracing. Azonnal érvényes.'},
 'it': {'tooltip_cr01_ambientocclusion': "L'occlusione ambientale del gioco. Disattivato può aumentare gli FPS. Solo "
                                         'Path Tracing. Si applica subito.',
        'tooltip_cr01_aocontrast': "Contrasto dell'occlusione ambientale. Preferenza visiva; nessun vantaggio di FPS "
                                   'accertato. Solo Path Tracing. Si applica subito.',
        'tooltip_cr01_aoradius': "Fin dove arriva l'occlusione ambientale. Preferenza visiva; nessun vantaggio di FPS "
                                 'accertato. Solo Path Tracing. Si applica subito.',
        'tooltip_cr01_aostrength': "Quanto appare scura l'occlusione ambientale. Preferenza visiva; nessun vantaggio "
                                   'di FPS accertato. Solo Path Tracing. Si applica subito.',
        'tooltip_cr01_cpu_distant_trees': 'Ottimizza i billboard degli alberi lontani. In pausa nel Path Tracing '
                                          'finché PT: Billboards è attivo. Attivato può aumentare gli FPS. Ray Tracing '
                                          'normale e Path Tracing. Si applica alle aree caricate di nuovo.',
        'tooltip_cr01_cpu_fast_bone_reads': 'Velocizza la lettura da parte della CPU delle ossa di animazione di '
                                            "personaggi e creature; l'animazione resta uguale. Attivato può aumentare "
                                            'gli FPS. Si applica subito.',
        'tooltip_cr01_cpu_physics_lod': 'Riduce il lavoro della CPU sui dettagli della fisica; oggetti lontani e '
                                        'stoffe possono muoversi con meno dettaglio. Attivato può aumentare gli FPS. '
                                        'Si applica subito.',
        'tooltip_cr01_geometrybounces': 'Quante volte la luce rimbalza sulle superfici. Valori più bassi possono '
                                        'aumentare gli FPS. Solo Path Tracing. Si applica subito.',
        'tooltip_cr01_grassday': "Luminosità dell'erba di giorno. Preferenza visiva; nessun vantaggio di FPS "
                                 'accertato. Solo Path Tracing. Si applica subito.',
        'tooltip_cr01_grassnight': "Luminosità dell'erba di notte. Preferenza visiva; nessun vantaggio di FPS "
                                   'accertato. Solo Path Tracing. Si applica subito.',
        'tooltip_cr01_hairbounces': 'Quante volte la luce rimbalza dentro i capelli. Valori più bassi possono '
                                    'aumentare gli FPS. Solo Path Tracing. Si applica subito.',
        'tooltip_cr01_indirectlight': 'Luminosità della luce indiretta (1.00 = neutro). Preferenza visiva; nessun '
                                      'vantaggio di FPS accertato. Solo Path Tracing. Si applica subito.',
        'tooltip_cr01_pt_scene_range': "Riduce l'area in ray tracing intorno a te (gioco: 400 m); riflessi, luce "
                                       'indiretta e ombre lontane possono diminuire. Valori più bassi possono '
                                       'aumentare gli FPS. Solo Path Tracing. Si applica subito.',
        'tooltip_cr01_ptbillboards': 'Ombre dei billboard degli alberi lontani. Attivato mette in pausa CPU: Distant '
                                     'Trees. Disattivato può aumentare gli FPS. Solo Path Tracing. Si applica alle '
                                     'aree caricate di nuovo.',
        'tooltip_cr01_ptshadows': "Ombre in ray tracing; Disattivato perde dettaglio d'ombra. Disattivato può "
                                  'aumentare gli FPS. Solo Path Tracing. Si applica subito.',
        'tooltip_cr01_rt_perf_ao': 'Qualità del pre-tracciamento delle ombre di contatto. Valori più bassi possono '
                                   'aumentare gli FPS. Solo Ray Tracing normale. Si applica subito. Richiede '
                                   'Ray-Traced Optimizations su Auto.',
        'tooltip_cr01_rt_perf_budget': 'Lavoro di illuminazione per fotogramma; budget bassi possono ritardare la luce '
                                       "o sbiadire l'erba. Valori più bassi possono aumentare gli FPS. Solo Ray "
                                       'Tracing normale. Si applica subito. Richiede Ray-Traced Optimizations su Auto.',
        'tooltip_cr01_rt_perf_cap': 'Limita quante sonde di luce vengono classificate; limiti bassi possono ritardare '
                                    "l'illuminazione. Valori più bassi possono aumentare gli FPS. Solo Ray Tracing "
                                    'normale. Si applica subito. Richiede Ray-Traced Optimizations su Auto.',
        'tooltip_cr01_rt_perf_global_shading_limit': 'Sonde della griglia globale elaborate per fotogramma; limiti '
                                                     "bassi possono ritardare l'illuminazione. Valori più bassi "
                                                     'possono aumentare gli FPS. Solo Ray Tracing normale. Si applica '
                                                     'subito. Richiede Ray-Traced Optimizations su Auto.',
        'tooltip_cr01_rt_perf_grid1_shading_limit': 'Sonde della griglia secondaria elaborate per fotogramma; limiti '
                                                    "bassi possono ritardare l'illuminazione. Valori più bassi possono "
                                                    'aumentare gli FPS. Solo Ray Tracing normale. Si applica subito. '
                                                    'Richiede Ray-Traced Optimizations su Auto.',
        'tooltip_cr01_rt_perf_primary': "Raggi di classificazione primari dell'illuminazione globale; meno raggi "
                                        "possono cambiare l'illuminazione. Valori più bassi possono aumentare gli FPS. "
                                        'Solo Ray Tracing normale. Si applica subito. Richiede Ray-Traced '
                                        'Optimizations su Auto.',
        'tooltip_cr01_rt_perf_secondary': "Raggi di classificazione secondari dell'illuminazione globale; meno raggi "
                                          "possono cambiare l'illuminazione. Valori più bassi possono aumentare gli "
                                          'FPS. Solo Ray Tracing normale. Si applica subito. Richiede Ray-Traced '
                                          'Optimizations su Auto.',
        'tooltip_cr01_rt_quality_ao': 'Qualità del pre-tracciamento delle ombre di contatto. Valori più bassi possono '
                                      'aumentare gli FPS. Solo Ray Tracing normale. Si applica subito. Richiede '
                                      'Ray-Traced Optimizations su Auto.',
        'tooltip_cr01_rt_quality_budget': 'Lavoro di illuminazione per fotogramma; budget bassi possono ritardare la '
                                          "luce o sbiadire l'erba. Valori più bassi possono aumentare gli FPS. Solo "
                                          'Ray Tracing normale. Si applica subito. Richiede Ray-Traced Optimizations '
                                          'su Auto.',
        'tooltip_cr01_rt_quality_cap': 'Limita quante sonde di luce vengono classificate; limiti bassi possono '
                                       "ritardare l'illuminazione. Valori più bassi possono aumentare gli FPS. Solo "
                                       'Ray Tracing normale. Si applica subito. Richiede Ray-Traced Optimizations su '
                                       'Auto.',
        'tooltip_cr01_rt_quality_global_shading_limit': 'Sonde della griglia globale elaborate per fotogramma; limiti '
                                                        "bassi possono ritardare l'illuminazione. Valori più bassi "
                                                        'possono aumentare gli FPS. Solo Ray Tracing normale. Si '
                                                        'applica subito. Richiede Ray-Traced Optimizations su Auto.',
        'tooltip_cr01_rt_quality_grid1_shading_limit': 'Sonde della griglia secondaria elaborate per fotogramma; '
                                                       "limiti bassi possono ritardare l'illuminazione. Valori più "
                                                       'bassi possono aumentare gli FPS. Solo Ray Tracing normale. Si '
                                                       'applica subito. Richiede Ray-Traced Optimizations su Auto.',
        'tooltip_cr01_rt_quality_primary': "Raggi di classificazione primari dell'illuminazione globale; meno raggi "
                                           "possono cambiare l'illuminazione. Valori più bassi possono aumentare gli "
                                           'FPS. Solo Ray Tracing normale. Si applica subito. Richiede Ray-Traced '
                                           'Optimizations su Auto.',
        'tooltip_cr01_rt_quality_secondary': "Raggi di classificazione secondari dell'illuminazione globale; meno "
                                             "raggi possono cambiare l'illuminazione. Valori più bassi possono "
                                             'aumentare gli FPS. Solo Ray Tracing normale. Si applica subito. Richiede '
                                             'Ray-Traced Optimizations su Auto.',
        'tooltip_cr01_rt_refl_local_lights': "Disattivato ignora lampade, torce e fuochi nell'illuminare i riflessi; "
                                             'le zone illuminate appaiono più scure. Disattivato può aumentare gli '
                                             'FPS. Solo Ray Tracing normale. Si applica subito.',
        'tooltip_cr01_rt_shadow_foliage_detail': "Usa texture delle foglie più grossolane per i raggi d'ombra; le "
                                                 'ombre delle foglie diventano più morbide. Meno dettaglio può '
                                                 'aumentare gli FPS. Solo Ray Tracing normale. Si applica subito.',
        'tooltip_cr01_rt_shadow_local_lights': 'Quante luci vicine proiettano ombre in ray tracing (gioco: 8); le '
                                               'ombre del sole non cambiano. Valori più bassi possono aumentare gli '
                                               'FPS. Solo Ray Tracing normale. Si applica subito.',
        'tooltip_cr01_skyimpact': 'Forza della luce del cielo (1.00 = neutro). Preferenza visiva; nessun vantaggio di '
                                  'FPS accertato. Solo Path Tracing. Si applica subito.',
        'tooltip_cr01_specularcheckerboard': 'Campionamento a scacchiera per alcuni riflessi; Attivato può perdere un '
                                             "po' di dettaglio. Attivato può aumentare gli FPS. Solo Path Tracing. Si "
                                             'applica subito.',
        'tooltip_cr01_specularscreenspace': 'Aiuto nello spazio schermo per alcuni riflessi; Disattivato può perdere '
                                            'dettaglio. Disattivato può aumentare gli FPS. Solo Path Tracing. Si '
                                            'applica subito.',
        'tooltip_cr01_sunimpact': 'Forza della luce solare (1.00 = neutro). Preferenza visiva; nessun vantaggio di FPS '
                                  'accertato. Solo Path Tracing. Si applica subito.'},
 'jp': {'tooltip_cr01_ambientocclusion': 'ゲーム本来のアンビエントオクルージョンです。オフで FPS が向上する場合があります。Path Tracing のみ。すぐに反映されます。',
        'tooltip_cr01_aocontrast': 'アンビエントオクルージョンのコントラストです。見た目の好みの設定で、FPS 向上は確認されていません。Path Tracing のみ。すぐに反映されます。',
        'tooltip_cr01_aoradius': 'アンビエントオクルージョンが届く範囲です。見た目の好みの設定で、FPS 向上は確認されていません。Path Tracing のみ。すぐに反映されます。',
        'tooltip_cr01_aostrength': 'アンビエントオクルージョンの暗さです。見た目の好みの設定で、FPS 向上は確認されていません。Path Tracing のみ。すぐに反映されます。',
        'tooltip_cr01_cpu_distant_trees': '遠くの木のビルボードを最適化します。PT: Billboards がオンの間、Path Tracing では一時停止します。オンで FPS '
                                          'が向上する場合があります。通常の Ray Tracing と Path Tracing。新しく読み込まれたエリアに反映されます。',
        'tooltip_cr01_cpu_fast_bone_reads': 'キャラクターやクリーチャーのアニメーション用ボーンの CPU 読み取りを高速化します。アニメーションの見た目は変わりません。オンで FPS '
                                            'が向上する場合があります。すぐに反映されます。',
        'tooltip_cr01_cpu_physics_lod': '物理演算の詳細にかかる CPU 負荷を減らします。遠くの物体や布の動きが簡略になる場合があります。オンで FPS '
                                        'が向上する場合があります。すぐに反映されます。',
        'tooltip_cr01_geometrybounces': '光が表面で跳ね返る回数です。値を下げると FPS が向上する場合があります。Path Tracing のみ。すぐに反映されます。',
        'tooltip_cr01_grassday': '昼の草の明るさです。見た目の好みの設定で、FPS 向上は確認されていません。Path Tracing のみ。すぐに反映されます。',
        'tooltip_cr01_grassnight': '夜の草の明るさです。見た目の好みの設定で、FPS 向上は確認されていません。Path Tracing のみ。すぐに反映されます。',
        'tooltip_cr01_hairbounces': '光が髪の中で跳ね返る回数です。値を下げると FPS が向上する場合があります。Path Tracing のみ。すぐに反映されます。',
        'tooltip_cr01_indirectlight': '間接光の明るさです（1.00 = 標準）。見た目の好みの設定で、FPS 向上は確認されていません。Path Tracing のみ。すぐに反映されます。',
        'tooltip_cr01_pt_scene_range': '周囲のレイトレース範囲を狭めます（ゲーム既定: 400 m）。遠くの反射、間接光、影が弱まる場合があります。値を下げると FPS '
                                       'が向上する場合があります。Path Tracing のみ。すぐに反映されます。',
        'tooltip_cr01_ptbillboards': '遠くの木のビルボードの影です。オンにすると CPU: Distant Trees が一時停止します。オフで FPS が向上する場合があります。Path '
                                     'Tracing のみ。新しく読み込まれたエリアに反映されます。',
        'tooltip_cr01_ptshadows': 'レイトレースの影です。オフでは影の詳細が失われます。オフで FPS が向上する場合があります。Path Tracing のみ。すぐに反映されます。',
        'tooltip_cr01_rt_perf_ao': '接触影のプリトレースの品質です。値を下げると FPS が向上する場合があります。通常の Ray Tracing のみ。すぐに反映されます。Ray-Traced '
                                   'Optimizations を Auto にする必要があります。',
        'tooltip_cr01_rt_perf_budget': '1 フレームあたりのライティング処理量です。低いと光の更新が遅れたり草が色あせたりする場合があります。値を下げると FPS が向上する場合があります。通常の '
                                       'Ray Tracing のみ。すぐに反映されます。Ray-Traced Optimizations を Auto にする必要があります。',
        'tooltip_cr01_rt_perf_cap': '分類するライトプローブ数の上限です。低いとライティングの更新が遅れる場合があります。値を下げると FPS が向上する場合があります。通常の Ray Tracing '
                                    'のみ。すぐに反映されます。Ray-Traced Optimizations を Auto にする必要があります。',
        'tooltip_cr01_rt_perf_global_shading_limit': 'グローバルグリッドで 1 フレームに処理するプローブ数です。低いとライティングの更新が遅れる場合があります。値を下げると FPS '
                                                     'が向上する場合があります。通常の Ray Tracing のみ。すぐに反映されます。Ray-Traced '
                                                     'Optimizations を Auto にする必要があります。',
        'tooltip_cr01_rt_perf_grid1_shading_limit': 'セカンダリグリッドで 1 フレームに処理するプローブ数です。低いとライティングの更新が遅れる場合があります。値を下げると FPS '
                                                    'が向上する場合があります。通常の Ray Tracing のみ。すぐに反映されます。Ray-Traced '
                                                    'Optimizations を Auto にする必要があります。',
        'tooltip_cr01_rt_perf_primary': 'グローバルイルミネーションの一次分類レイです。減らすとライティングが変わる場合があります。値を下げると FPS が向上する場合があります。通常の Ray '
                                        'Tracing のみ。すぐに反映されます。Ray-Traced Optimizations を Auto にする必要があります。',
        'tooltip_cr01_rt_perf_secondary': 'グローバルイルミネーションの二次分類レイです。減らすとライティングが変わる場合があります。値を下げると FPS が向上する場合があります。通常の '
                                          'Ray Tracing のみ。すぐに反映されます。Ray-Traced Optimizations を Auto にする必要があります。',
        'tooltip_cr01_rt_quality_ao': '接触影のプリトレースの品質です。値を下げると FPS が向上する場合があります。通常の Ray Tracing のみ。すぐに反映されます。Ray-Traced '
                                      'Optimizations を Auto にする必要があります。',
        'tooltip_cr01_rt_quality_budget': '1 フレームあたりのライティング処理量です。低いと光の更新が遅れたり草が色あせたりする場合があります。値を下げると FPS '
                                          'が向上する場合があります。通常の Ray Tracing のみ。すぐに反映されます。Ray-Traced Optimizations を Auto '
                                          'にする必要があります。',
        'tooltip_cr01_rt_quality_cap': '分類するライトプローブ数の上限です。低いとライティングの更新が遅れる場合があります。値を下げると FPS が向上する場合があります。通常の Ray '
                                       'Tracing のみ。すぐに反映されます。Ray-Traced Optimizations を Auto にする必要があります。',
        'tooltip_cr01_rt_quality_global_shading_limit': 'グローバルグリッドで 1 フレームに処理するプローブ数です。低いとライティングの更新が遅れる場合があります。値を下げると '
                                                        'FPS が向上する場合があります。通常の Ray Tracing のみ。すぐに反映されます。Ray-Traced '
                                                        'Optimizations を Auto にする必要があります。',
        'tooltip_cr01_rt_quality_grid1_shading_limit': 'セカンダリグリッドで 1 フレームに処理するプローブ数です。低いとライティングの更新が遅れる場合があります。値を下げると '
                                                       'FPS が向上する場合があります。通常の Ray Tracing のみ。すぐに反映されます。Ray-Traced '
                                                       'Optimizations を Auto にする必要があります。',
        'tooltip_cr01_rt_quality_primary': 'グローバルイルミネーションの一次分類レイです。減らすとライティングが変わる場合があります。値を下げると FPS が向上する場合があります。通常の '
                                           'Ray Tracing のみ。すぐに反映されます。Ray-Traced Optimizations を Auto にする必要があります。',
        'tooltip_cr01_rt_quality_secondary': 'グローバルイルミネーションの二次分類レイです。減らすとライティングが変わる場合があります。値を下げると FPS が向上する場合があります。通常の '
                                             'Ray Tracing のみ。すぐに反映されます。Ray-Traced Optimizations を Auto にする必要があります。',
        'tooltip_cr01_rt_refl_local_lights': 'オフでは反射の照明でランプ、たいまつ、火を無視します。明るい場所が反射内で暗く見えます。オフで FPS が向上する場合があります。通常の Ray '
                                             'Tracing のみ。すぐに反映されます。',
        'tooltip_cr01_rt_shadow_foliage_detail': '影のレイに粗い葉のテクスチャを使います。葉の影が柔らかくなります。詳細を下げると FPS が向上する場合があります。通常の Ray '
                                                 'Tracing のみ。すぐに反映されます。',
        'tooltip_cr01_rt_shadow_local_lights': 'レイトレースの影を落とす近くの光源の数です（ゲーム既定: 8）。太陽の影は変わりません。値を下げると FPS '
                                               'が向上する場合があります。通常の Ray Tracing のみ。すぐに反映されます。',
        'tooltip_cr01_skyimpact': '空の光の強さです（1.00 = 標準）。見た目の好みの設定で、FPS 向上は確認されていません。Path Tracing のみ。すぐに反映されます。',
        'tooltip_cr01_specularcheckerboard': '一部の反射のチェッカーボードサンプリングです。オンでは詳細が少し失われる場合があります。オンで FPS が向上する場合があります。Path '
                                             'Tracing のみ。すぐに反映されます。',
        'tooltip_cr01_specularscreenspace': '一部の反射のスクリーンスペース補助です。オフでは反射の詳細が失われる場合があります。オフで FPS が向上する場合があります。Path '
                                            'Tracing のみ。すぐに反映されます。',
        'tooltip_cr01_sunimpact': '日光の強さです（1.00 = 標準）。見た目の好みの設定で、FPS 向上は確認されていません。Path Tracing のみ。すぐに反映されます。'},
 'kr': {'tooltip_cr01_ambientocclusion': '게임의 앰비언트 오클루전입니다. 끄기로 FPS가 오를 수 있습니다. Path Tracing 전용입니다. 즉시 적용됩니다.',
        'tooltip_cr01_aocontrast': '앰비언트 오클루전의 대비입니다. 외형 취향 설정이며 확인된 FPS 이점은 없습니다. Path Tracing 전용입니다. 즉시 적용됩니다.',
        'tooltip_cr01_aoradius': '앰비언트 오클루전이 미치는 범위입니다. 외형 취향 설정이며 확인된 FPS 이점은 없습니다. Path Tracing 전용입니다. 즉시 적용됩니다.',
        'tooltip_cr01_aostrength': '앰비언트 오클루전의 어두운 정도입니다. 외형 취향 설정이며 확인된 FPS 이점은 없습니다. Path Tracing 전용입니다. 즉시 적용됩니다.',
        'tooltip_cr01_cpu_distant_trees': '먼 나무 빌보드를 최적화합니다. PT: Billboards가 켜져 있는 동안 Path Tracing에서는 일시 중지됩니다. 켜기로 '
                                          'FPS가 오를 수 있습니다. 일반 Ray Tracing과 Path Tracing에 적용됩니다. 새로 불러온 지역에 적용됩니다.',
        'tooltip_cr01_cpu_fast_bone_reads': '캐릭터와 크리처 애니메이션 본의 CPU 읽기를 빠르게 합니다. 애니메이션은 그대로입니다. 켜기로 FPS가 오를 수 있습니다. 즉시 '
                                            '적용됩니다.',
        'tooltip_cr01_cpu_physics_lod': '물리 디테일에 드는 CPU 작업을 줄입니다. 먼 물체와 옷감의 움직임이 덜 세밀해질 수 있습니다. 켜기로 FPS가 오를 수 있습니다. 즉시 '
                                        '적용됩니다.',
        'tooltip_cr01_geometrybounces': '빛이 표면에서 튕기는 횟수입니다. 값을 낮추면 FPS가 오를 수 있습니다. Path Tracing 전용입니다. 즉시 적용됩니다.',
        'tooltip_cr01_grassday': '낮의 풀 밝기입니다. 외형 취향 설정이며 확인된 FPS 이점은 없습니다. Path Tracing 전용입니다. 즉시 적용됩니다.',
        'tooltip_cr01_grassnight': '밤의 풀 밝기입니다. 외형 취향 설정이며 확인된 FPS 이점은 없습니다. Path Tracing 전용입니다. 즉시 적용됩니다.',
        'tooltip_cr01_hairbounces': '빛이 머리카락 안에서 튕기는 횟수입니다. 값을 낮추면 FPS가 오를 수 있습니다. Path Tracing 전용입니다. 즉시 적용됩니다.',
        'tooltip_cr01_indirectlight': '간접광의 밝기입니다(1.00 = 기준). 외형 취향 설정이며 확인된 FPS 이점은 없습니다. Path Tracing 전용입니다. 즉시 '
                                      '적용됩니다.',
        'tooltip_cr01_pt_scene_range': '주변의 레이 트레이싱 범위를 줄입니다(게임 기본: 400 m). 먼 반사, 간접광, 그림자가 줄어들 수 있습니다. 값을 낮추면 FPS가 오를 '
                                       '수 있습니다. Path Tracing 전용입니다. 즉시 적용됩니다.',
        'tooltip_cr01_ptbillboards': '먼 나무 빌보드의 그림자입니다. 켜기는 CPU: Distant Trees를 일시 중지합니다. 끄기로 FPS가 오를 수 있습니다. Path '
                                     'Tracing 전용입니다. 새로 불러온 지역에 적용됩니다.',
        'tooltip_cr01_ptshadows': '레이 트레이싱 그림자입니다. 끄기는 그림자 디테일을 잃습니다. 끄기로 FPS가 오를 수 있습니다. Path Tracing 전용입니다. 즉시 '
                                  '적용됩니다.',
        'tooltip_cr01_rt_perf_ao': '접촉 그림자 사전 추적의 품질입니다. 값을 낮추면 FPS가 오를 수 있습니다. 일반 Ray Tracing 전용입니다. 즉시 적용됩니다. '
                                   'Ray-Traced Optimizations가 Auto여야 합니다.',
        'tooltip_cr01_rt_perf_budget': '프레임당 조명 작업량입니다. 낮으면 빛이 늦게 갱신되거나 풀이 바랠 수 있습니다. 값을 낮추면 FPS가 오를 수 있습니다. 일반 Ray '
                                       'Tracing 전용입니다. 즉시 적용됩니다. Ray-Traced Optimizations가 Auto여야 합니다.',
        'tooltip_cr01_rt_perf_cap': '분류되는 조명 프로브 수를 제한합니다. 낮으면 조명 갱신이 늦어질 수 있습니다. 값을 낮추면 FPS가 오를 수 있습니다. 일반 Ray '
                                    'Tracing 전용입니다. 즉시 적용됩니다. Ray-Traced Optimizations가 Auto여야 합니다.',
        'tooltip_cr01_rt_perf_global_shading_limit': '전역 그리드에서 프레임당 셰이딩하는 프로브 수입니다. 낮으면 조명 갱신이 늦어질 수 있습니다. 값을 낮추면 FPS가 '
                                                     '오를 수 있습니다. 일반 Ray Tracing 전용입니다. 즉시 적용됩니다. Ray-Traced '
                                                     'Optimizations가 Auto여야 합니다.',
        'tooltip_cr01_rt_perf_grid1_shading_limit': '보조 그리드에서 프레임당 셰이딩하는 프로브 수입니다. 낮으면 조명 갱신이 늦어질 수 있습니다. 값을 낮추면 FPS가 '
                                                    '오를 수 있습니다. 일반 Ray Tracing 전용입니다. 즉시 적용됩니다. Ray-Traced '
                                                    'Optimizations가 Auto여야 합니다.',
        'tooltip_cr01_rt_perf_primary': '전역 조명의 1차 분류 광선입니다. 줄이면 조명이 달라질 수 있습니다. 값을 낮추면 FPS가 오를 수 있습니다. 일반 Ray Tracing '
                                        '전용입니다. 즉시 적용됩니다. Ray-Traced Optimizations가 Auto여야 합니다.',
        'tooltip_cr01_rt_perf_secondary': '전역 조명의 2차 분류 광선입니다. 줄이면 조명이 달라질 수 있습니다. 값을 낮추면 FPS가 오를 수 있습니다. 일반 Ray '
                                          'Tracing 전용입니다. 즉시 적용됩니다. Ray-Traced Optimizations가 Auto여야 합니다.',
        'tooltip_cr01_rt_quality_ao': '접촉 그림자 사전 추적의 품질입니다. 값을 낮추면 FPS가 오를 수 있습니다. 일반 Ray Tracing 전용입니다. 즉시 적용됩니다. '
                                      'Ray-Traced Optimizations가 Auto여야 합니다.',
        'tooltip_cr01_rt_quality_budget': '프레임당 조명 작업량입니다. 낮으면 빛이 늦게 갱신되거나 풀이 바랠 수 있습니다. 값을 낮추면 FPS가 오를 수 있습니다. 일반 Ray '
                                          'Tracing 전용입니다. 즉시 적용됩니다. Ray-Traced Optimizations가 Auto여야 합니다.',
        'tooltip_cr01_rt_quality_cap': '분류되는 조명 프로브 수를 제한합니다. 낮으면 조명 갱신이 늦어질 수 있습니다. 값을 낮추면 FPS가 오를 수 있습니다. 일반 Ray '
                                       'Tracing 전용입니다. 즉시 적용됩니다. Ray-Traced Optimizations가 Auto여야 합니다.',
        'tooltip_cr01_rt_quality_global_shading_limit': '전역 그리드에서 프레임당 셰이딩하는 프로브 수입니다. 낮으면 조명 갱신이 늦어질 수 있습니다. 값을 낮추면 '
                                                        'FPS가 오를 수 있습니다. 일반 Ray Tracing 전용입니다. 즉시 적용됩니다. Ray-Traced '
                                                        'Optimizations가 Auto여야 합니다.',
        'tooltip_cr01_rt_quality_grid1_shading_limit': '보조 그리드에서 프레임당 셰이딩하는 프로브 수입니다. 낮으면 조명 갱신이 늦어질 수 있습니다. 값을 낮추면 '
                                                       'FPS가 오를 수 있습니다. 일반 Ray Tracing 전용입니다. 즉시 적용됩니다. Ray-Traced '
                                                       'Optimizations가 Auto여야 합니다.',
        'tooltip_cr01_rt_quality_primary': '전역 조명의 1차 분류 광선입니다. 줄이면 조명이 달라질 수 있습니다. 값을 낮추면 FPS가 오를 수 있습니다. 일반 Ray '
                                           'Tracing 전용입니다. 즉시 적용됩니다. Ray-Traced Optimizations가 Auto여야 합니다.',
        'tooltip_cr01_rt_quality_secondary': '전역 조명의 2차 분류 광선입니다. 줄이면 조명이 달라질 수 있습니다. 값을 낮추면 FPS가 오를 수 있습니다. 일반 Ray '
                                             'Tracing 전용입니다. 즉시 적용됩니다. Ray-Traced Optimizations가 Auto여야 합니다.',
        'tooltip_cr01_rt_refl_local_lights': '끄기는 반사를 비출 때 램프, 횃불, 불을 무시합니다. 밝은 곳이 반사에서 더 어둡게 보입니다. 끄기로 FPS가 오를 수 '
                                             '있습니다. 일반 Ray Tracing 전용입니다. 즉시 적용됩니다.',
        'tooltip_cr01_rt_shadow_foliage_detail': '그림자 광선에 더 거친 잎 텍스처를 사용합니다. 잎 그림자가 부드러워집니다. 디테일을 낮추면 FPS가 오를 수 있습니다. '
                                                 '일반 Ray Tracing 전용입니다. 즉시 적용됩니다.',
        'tooltip_cr01_rt_shadow_local_lights': '레이 트레이싱 그림자를 드리우는 가까운 광원 수입니다(게임 기본: 8). 태양 그림자는 그대로입니다. 값을 낮추면 FPS가 '
                                               '오를 수 있습니다. 일반 Ray Tracing 전용입니다. 즉시 적용됩니다.',
        'tooltip_cr01_skyimpact': '하늘빛의 세기입니다(1.00 = 기준). 외형 취향 설정이며 확인된 FPS 이점은 없습니다. Path Tracing 전용입니다. 즉시 적용됩니다.',
        'tooltip_cr01_specularcheckerboard': '일부 반사의 체커보드 샘플링입니다. 켜기는 디테일이 조금 줄 수 있습니다. 켜기로 FPS가 오를 수 있습니다. Path '
                                             'Tracing 전용입니다. 즉시 적용됩니다.',
        'tooltip_cr01_specularscreenspace': '일부 반사를 위한 화면 공간 보조입니다. 끄기는 반사 디테일을 잃을 수 있습니다. 끄기로 FPS가 오를 수 있습니다. Path '
                                            'Tracing 전용입니다. 즉시 적용됩니다.',
        'tooltip_cr01_sunimpact': '햇빛의 세기입니다(1.00 = 기준). 외형 취향 설정이며 확인된 FPS 이점은 없습니다. Path Tracing 전용입니다. 즉시 적용됩니다.'},
 'pl': {'tooltip_cr01_ambientocclusion': 'Okluzja otoczenia gry. Wyłączone może zwiększyć FPS. Tylko Path Tracing. '
                                         'Działa od razu.',
        'tooltip_cr01_aocontrast': 'Kontrast okluzji otoczenia. Kwestia wyglądu; brak potwierdzonego zysku FPS. Tylko '
                                   'Path Tracing. Działa od razu.',
        'tooltip_cr01_aoradius': 'Jak daleko sięga okluzja otoczenia. Kwestia wyglądu; brak potwierdzonego zysku FPS. '
                                 'Tylko Path Tracing. Działa od razu.',
        'tooltip_cr01_aostrength': 'Jak ciemna jest okluzja otoczenia. Kwestia wyglądu; brak potwierdzonego zysku FPS. '
                                   'Tylko Path Tracing. Działa od razu.',
        'tooltip_cr01_cpu_distant_trees': 'Optymalizuje billboardy odległych drzew. Wstrzymane w Path Tracingu, gdy '
                                          'PT: Billboards jest włączone. Włączone może zwiększyć FPS. Zwykły Ray '
                                          'Tracing i Path Tracing. Działa w nowo wczytanych obszarach.',
        'tooltip_cr01_cpu_fast_bone_reads': 'Przyspiesza odczyt kości animacji postaci i stworzeń przez CPU; animacja '
                                            'wygląda tak samo. Włączone może zwiększyć FPS. Działa od razu.',
        'tooltip_cr01_cpu_physics_lod': 'Zmniejsza pracę CPU nad szczegółami fizyki; odległe obiekty i tkaniny mogą '
                                        'poruszać się mniej szczegółowo. Włączone może zwiększyć FPS. Działa od razu.',
        'tooltip_cr01_geometrybounces': 'Ile razy światło odbija się od powierzchni. Niższe wartości mogą zwiększyć '
                                        'FPS. Tylko Path Tracing. Działa od razu.',
        'tooltip_cr01_grassday': 'Jasność trawy w dzień. Kwestia wyglądu; brak potwierdzonego zysku FPS. Tylko Path '
                                 'Tracing. Działa od razu.',
        'tooltip_cr01_grassnight': 'Jasność trawy w nocy. Kwestia wyglądu; brak potwierdzonego zysku FPS. Tylko Path '
                                   'Tracing. Działa od razu.',
        'tooltip_cr01_hairbounces': 'Ile razy światło odbija się wewnątrz włosów. Niższe wartości mogą zwiększyć FPS. '
                                    'Tylko Path Tracing. Działa od razu.',
        'tooltip_cr01_indirectlight': 'Jasność światła pośredniego (1.00 = neutralnie). Kwestia wyglądu; brak '
                                      'potwierdzonego zysku FPS. Tylko Path Tracing. Działa od razu.',
        'tooltip_cr01_pt_scene_range': 'Zmniejsza obszar ray tracingu wokół ciebie (gra: 400 m); odległe odbicia, '
                                       'światło pośrednie i cienie mogą osłabnąć. Niższe wartości mogą zwiększyć FPS. '
                                       'Tylko Path Tracing. Działa od razu.',
        'tooltip_cr01_ptbillboards': 'Cienie billboardów odległych drzew. Włączone wstrzymuje CPU: Distant Trees. '
                                     'Wyłączone może zwiększyć FPS. Tylko Path Tracing. Działa w nowo wczytanych '
                                     'obszarach.',
        'tooltip_cr01_ptshadows': 'Cienie z ray tracingiem; Wyłączone traci szczegóły cieni. Wyłączone może zwiększyć '
                                  'FPS. Tylko Path Tracing. Działa od razu.',
        'tooltip_cr01_rt_perf_ao': 'Jakość wstępnego śledzenia cieni kontaktowych. Niższe wartości mogą zwiększyć FPS. '
                                   'Tylko zwykły Ray Tracing. Działa od razu. Wymaga Ray-Traced Optimizations '
                                   'ustawionego na Auto.',
        'tooltip_cr01_rt_perf_budget': 'Praca oświetlenia na klatkę; niski budżet może opóźniać światło lub blaknięcie '
                                       'trawy. Niższe wartości mogą zwiększyć FPS. Tylko zwykły Ray Tracing. Działa od '
                                       'razu. Wymaga Ray-Traced Optimizations ustawionego na Auto.',
        'tooltip_cr01_rt_perf_cap': 'Ogranicza liczbę klasyfikowanych sond światła; niskie limity mogą opóźniać '
                                    'oświetlenie. Niższe wartości mogą zwiększyć FPS. Tylko zwykły Ray Tracing. Działa '
                                    'od razu. Wymaga Ray-Traced Optimizations ustawionego na Auto.',
        'tooltip_cr01_rt_perf_global_shading_limit': 'Sondy siatki globalnej cieniowane na klatkę; niskie limity mogą '
                                                     'opóźniać oświetlenie. Niższe wartości mogą zwiększyć FPS. Tylko '
                                                     'zwykły Ray Tracing. Działa od razu. Wymaga Ray-Traced '
                                                     'Optimizations ustawionego na Auto.',
        'tooltip_cr01_rt_perf_grid1_shading_limit': 'Sondy siatki dodatkowej cieniowane na klatkę; niskie limity mogą '
                                                    'opóźniać oświetlenie. Niższe wartości mogą zwiększyć FPS. Tylko '
                                                    'zwykły Ray Tracing. Działa od razu. Wymaga Ray-Traced '
                                                    'Optimizations ustawionego na Auto.',
        'tooltip_cr01_rt_perf_primary': 'Główne promienie klasyfikacji oświetlenia globalnego; mniej promieni może '
                                        'zmienić oświetlenie. Niższe wartości mogą zwiększyć FPS. Tylko zwykły Ray '
                                        'Tracing. Działa od razu. Wymaga Ray-Traced Optimizations ustawionego na Auto.',
        'tooltip_cr01_rt_perf_secondary': 'Dodatkowe promienie klasyfikacji oświetlenia globalnego; mniej promieni '
                                          'może zmienić oświetlenie. Niższe wartości mogą zwiększyć FPS. Tylko zwykły '
                                          'Ray Tracing. Działa od razu. Wymaga Ray-Traced Optimizations ustawionego na '
                                          'Auto.',
        'tooltip_cr01_rt_quality_ao': 'Jakość wstępnego śledzenia cieni kontaktowych. Niższe wartości mogą zwiększyć '
                                      'FPS. Tylko zwykły Ray Tracing. Działa od razu. Wymaga Ray-Traced Optimizations '
                                      'ustawionego na Auto.',
        'tooltip_cr01_rt_quality_budget': 'Praca oświetlenia na klatkę; niski budżet może opóźniać światło lub '
                                          'blaknięcie trawy. Niższe wartości mogą zwiększyć FPS. Tylko zwykły Ray '
                                          'Tracing. Działa od razu. Wymaga Ray-Traced Optimizations ustawionego na '
                                          'Auto.',
        'tooltip_cr01_rt_quality_cap': 'Ogranicza liczbę klasyfikowanych sond światła; niskie limity mogą opóźniać '
                                       'oświetlenie. Niższe wartości mogą zwiększyć FPS. Tylko zwykły Ray Tracing. '
                                       'Działa od razu. Wymaga Ray-Traced Optimizations ustawionego na Auto.',
        'tooltip_cr01_rt_quality_global_shading_limit': 'Sondy siatki globalnej cieniowane na klatkę; niskie limity '
                                                        'mogą opóźniać oświetlenie. Niższe wartości mogą zwiększyć '
                                                        'FPS. Tylko zwykły Ray Tracing. Działa od razu. Wymaga '
                                                        'Ray-Traced Optimizations ustawionego na Auto.',
        'tooltip_cr01_rt_quality_grid1_shading_limit': 'Sondy siatki dodatkowej cieniowane na klatkę; niskie limity '
                                                       'mogą opóźniać oświetlenie. Niższe wartości mogą zwiększyć FPS. '
                                                       'Tylko zwykły Ray Tracing. Działa od razu. Wymaga Ray-Traced '
                                                       'Optimizations ustawionego na Auto.',
        'tooltip_cr01_rt_quality_primary': 'Główne promienie klasyfikacji oświetlenia globalnego; mniej promieni może '
                                           'zmienić oświetlenie. Niższe wartości mogą zwiększyć FPS. Tylko zwykły Ray '
                                           'Tracing. Działa od razu. Wymaga Ray-Traced Optimizations ustawionego na '
                                           'Auto.',
        'tooltip_cr01_rt_quality_secondary': 'Dodatkowe promienie klasyfikacji oświetlenia globalnego; mniej promieni '
                                             'może zmienić oświetlenie. Niższe wartości mogą zwiększyć FPS. Tylko '
                                             'zwykły Ray Tracing. Działa od razu. Wymaga Ray-Traced Optimizations '
                                             'ustawionego na Auto.',
        'tooltip_cr01_rt_refl_local_lights': 'Wyłączone pomija lampy, pochodnie i ogniska przy oświetlaniu odbić; '
                                             'oświetlone miejsca są w nich ciemniejsze. Wyłączone może zwiększyć FPS. '
                                             'Tylko zwykły Ray Tracing. Działa od razu.',
        'tooltip_cr01_rt_shadow_foliage_detail': 'Używa grubszych tekstur liści dla promieni cieni; cienie liści stają '
                                                 'się miękkie. Mniej szczegółów może zwiększyć FPS. Tylko zwykły Ray '
                                                 'Tracing. Działa od razu.',
        'tooltip_cr01_rt_shadow_local_lights': 'Ile pobliskich świateł rzuca cienie z ray tracingiem (gra: 8); cienie '
                                               'słońca się nie zmieniają. Niższe wartości mogą zwiększyć FPS. Tylko '
                                               'zwykły Ray Tracing. Działa od razu.',
        'tooltip_cr01_skyimpact': 'Siła światła nieba (1.00 = neutralnie). Kwestia wyglądu; brak potwierdzonego zysku '
                                  'FPS. Tylko Path Tracing. Działa od razu.',
        'tooltip_cr01_specularcheckerboard': 'Próbkowanie szachownicowe dla części odbić; Włączone może utracić nieco '
                                             'szczegółów. Włączone może zwiększyć FPS. Tylko Path Tracing. Działa od '
                                             'razu.',
        'tooltip_cr01_specularscreenspace': 'Pomoc w przestrzeni ekranu dla części odbić; Wyłączone może utracić '
                                            'szczegóły. Wyłączone może zwiększyć FPS. Tylko Path Tracing. Działa od '
                                            'razu.',
        'tooltip_cr01_sunimpact': 'Siła światła słonecznego (1.00 = neutralnie). Kwestia wyglądu; brak potwierdzonego '
                                  'zysku FPS. Tylko Path Tracing. Działa od razu.'},
 'ru': {'tooltip_cr01_ambientocclusion': 'Фоновое затенение игры. Выкл. может повысить FPS. Только Path Tracing. '
                                         'Применяется сразу.',
        'tooltip_cr01_aocontrast': 'Контраст фонового затенения. Дело вкуса; подтверждённого прироста FPS нет. Только '
                                   'Path Tracing. Применяется сразу.',
        'tooltip_cr01_aoradius': 'Насколько далеко распространяется фоновое затенение. Дело вкуса; подтверждённого '
                                 'прироста FPS нет. Только Path Tracing. Применяется сразу.',
        'tooltip_cr01_aostrength': 'Насколько тёмным выглядит фоновое затенение. Дело вкуса; подтверждённого прироста '
                                   'FPS нет. Только Path Tracing. Применяется сразу.',
        'tooltip_cr01_cpu_distant_trees': 'Оптимизирует билборды дальних деревьев. В Path Tracing приостанавливается, '
                                          'пока включено PT: Billboards. Вкл. может повысить FPS. Обычная трассировка '
                                          'лучей и Path Tracing. Применяется к заново загруженным областям.',
        'tooltip_cr01_cpu_fast_bone_reads': 'Ускоряет чтение процессором костей анимации персонажей и существ; '
                                            'анимация выглядит так же. Вкл. может повысить FPS. Применяется сразу.',
        'tooltip_cr01_cpu_physics_lod': 'Снижает нагрузку процессора на детали физики; дальние объекты и ткань могут '
                                        'двигаться менее детально. Вкл. может повысить FPS. Применяется сразу.',
        'tooltip_cr01_geometrybounces': 'Сколько раз свет отражается от поверхностей. Меньшие значения могут повысить '
                                        'FPS. Только Path Tracing. Применяется сразу.',
        'tooltip_cr01_grassday': 'Яркость травы днём. Дело вкуса; подтверждённого прироста FPS нет. Только Path '
                                 'Tracing. Применяется сразу.',
        'tooltip_cr01_grassnight': 'Яркость травы ночью. Дело вкуса; подтверждённого прироста FPS нет. Только Path '
                                   'Tracing. Применяется сразу.',
        'tooltip_cr01_hairbounces': 'Сколько раз свет отражается внутри волос. Меньшие значения могут повысить FPS. '
                                    'Только Path Tracing. Применяется сразу.',
        'tooltip_cr01_indirectlight': 'Яркость непрямого света (1.00 = нейтрально). Дело вкуса; подтверждённого '
                                      'прироста FPS нет. Только Path Tracing. Применяется сразу.',
        'tooltip_cr01_pt_scene_range': 'Уменьшает область трассировки вокруг вас (игра: 400 м); дальние отражения, '
                                       'непрямой свет и тени могут ослабнуть. Меньшие значения могут повысить FPS. '
                                       'Только Path Tracing. Применяется сразу.',
        'tooltip_cr01_ptbillboards': 'Тени билбордов дальних деревьев. Вкл. приостанавливает CPU: Distant Trees. Выкл. '
                                     'может повысить FPS. Только Path Tracing. Применяется к заново загруженным '
                                     'областям.',
        'tooltip_cr01_ptshadows': 'Тени с трассировкой лучей; Выкл. теряет детализацию теней. Выкл. может повысить '
                                  'FPS. Только Path Tracing. Применяется сразу.',
        'tooltip_cr01_rt_perf_ao': 'Качество предварительной трассировки контактных теней. Меньшие значения могут '
                                   'повысить FPS. Только обычная трассировка лучей. Применяется сразу. Нужно '
                                   'Ray-Traced Optimizations в режиме Auto.',
        'tooltip_cr01_rt_perf_budget': 'Работа освещения за кадр; низкий бюджет может задерживать свет или делать '
                                       'траву блёклой. Меньшие значения могут повысить FPS. Только обычная трассировка '
                                       'лучей. Применяется сразу. Нужно Ray-Traced Optimizations в режиме Auto.',
        'tooltip_cr01_rt_perf_cap': 'Ограничивает число классифицируемых световых зондов; низкие пределы могут '
                                    'задерживать обновление освещения. Меньшие значения могут повысить FPS. Только '
                                    'обычная трассировка лучей. Применяется сразу. Нужно Ray-Traced Optimizations в '
                                    'режиме Auto.',
        'tooltip_cr01_rt_perf_global_shading_limit': 'Зонды глобальной сетки, затеняемые за кадр; низкие пределы могут '
                                                     'задерживать обновление освещения. Меньшие значения могут '
                                                     'повысить FPS. Только обычная трассировка лучей. Применяется '
                                                     'сразу. Нужно Ray-Traced Optimizations в режиме Auto.',
        'tooltip_cr01_rt_perf_grid1_shading_limit': 'Зонды вторичной сетки, затеняемые за кадр; низкие пределы могут '
                                                    'задерживать обновление освещения. Меньшие значения могут повысить '
                                                    'FPS. Только обычная трассировка лучей. Применяется сразу. Нужно '
                                                    'Ray-Traced Optimizations в режиме Auto.',
        'tooltip_cr01_rt_perf_primary': 'Первичные лучи классификации глобального освещения; меньше лучей может '
                                        'изменить освещение. Меньшие значения могут повысить FPS. Только обычная '
                                        'трассировка лучей. Применяется сразу. Нужно Ray-Traced Optimizations в режиме '
                                        'Auto.',
        'tooltip_cr01_rt_perf_secondary': 'Вторичные лучи классификации глобального освещения; меньше лучей может '
                                          'изменить освещение. Меньшие значения могут повысить FPS. Только обычная '
                                          'трассировка лучей. Применяется сразу. Нужно Ray-Traced Optimizations в '
                                          'режиме Auto.',
        'tooltip_cr01_rt_quality_ao': 'Качество предварительной трассировки контактных теней. Меньшие значения могут '
                                      'повысить FPS. Только обычная трассировка лучей. Применяется сразу. Нужно '
                                      'Ray-Traced Optimizations в режиме Auto.',
        'tooltip_cr01_rt_quality_budget': 'Работа освещения за кадр; низкий бюджет может задерживать свет или делать '
                                          'траву блёклой. Меньшие значения могут повысить FPS. Только обычная '
                                          'трассировка лучей. Применяется сразу. Нужно Ray-Traced Optimizations в '
                                          'режиме Auto.',
        'tooltip_cr01_rt_quality_cap': 'Ограничивает число классифицируемых световых зондов; низкие пределы могут '
                                       'задерживать обновление освещения. Меньшие значения могут повысить FPS. Только '
                                       'обычная трассировка лучей. Применяется сразу. Нужно Ray-Traced Optimizations в '
                                       'режиме Auto.',
        'tooltip_cr01_rt_quality_global_shading_limit': 'Зонды глобальной сетки, затеняемые за кадр; низкие пределы '
                                                        'могут задерживать обновление освещения. Меньшие значения '
                                                        'могут повысить FPS. Только обычная трассировка лучей. '
                                                        'Применяется сразу. Нужно Ray-Traced Optimizations в режиме '
                                                        'Auto.',
        'tooltip_cr01_rt_quality_grid1_shading_limit': 'Зонды вторичной сетки, затеняемые за кадр; низкие пределы '
                                                       'могут задерживать обновление освещения. Меньшие значения могут '
                                                       'повысить FPS. Только обычная трассировка лучей. Применяется '
                                                       'сразу. Нужно Ray-Traced Optimizations в режиме Auto.',
        'tooltip_cr01_rt_quality_primary': 'Первичные лучи классификации глобального освещения; меньше лучей может '
                                           'изменить освещение. Меньшие значения могут повысить FPS. Только обычная '
                                           'трассировка лучей. Применяется сразу. Нужно Ray-Traced Optimizations в '
                                           'режиме Auto.',
        'tooltip_cr01_rt_quality_secondary': 'Вторичные лучи классификации глобального освещения; меньше лучей может '
                                             'изменить освещение. Меньшие значения могут повысить FPS. Только обычная '
                                             'трассировка лучей. Применяется сразу. Нужно Ray-Traced Optimizations в '
                                             'режиме Auto.',
        'tooltip_cr01_rt_refl_local_lights': 'Выкл. не учитывает лампы, факелы и костры при освещении отражений; '
                                             'освещённые места в них темнее. Выкл. может повысить FPS. Только обычная '
                                             'трассировка лучей. Применяется сразу.',
        'tooltip_cr01_rt_shadow_foliage_detail': 'Использует более грубые текстуры листвы для теневых лучей; тени '
                                                 'листвы становятся мягче. Меньшая детализация может повысить FPS. '
                                                 'Только обычная трассировка лучей. Применяется сразу.',
        'tooltip_cr01_rt_shadow_local_lights': 'Сколько ближайших источников света отбрасывают тени с трассировкой '
                                               '(игра: 8); тени от солнца не меняются. Меньшие значения могут повысить '
                                               'FPS. Только обычная трассировка лучей. Применяется сразу.',
        'tooltip_cr01_skyimpact': 'Сила света неба (1.00 = нейтрально). Дело вкуса; подтверждённого прироста FPS нет. '
                                  'Только Path Tracing. Применяется сразу.',
        'tooltip_cr01_specularcheckerboard': 'Шахматная выборка для части отражений; Вкл. может немного потерять '
                                             'детали. Вкл. может повысить FPS. Только Path Tracing. Применяется сразу.',
        'tooltip_cr01_specularscreenspace': 'Помощь в экранном пространстве для части отражений; Выкл. может потерять '
                                            'детали. Выкл. может повысить FPS. Только Path Tracing. Применяется сразу.',
        'tooltip_cr01_sunimpact': 'Сила солнечного света (1.00 = нейтрально). Дело вкуса; подтверждённого прироста FPS '
                                  'нет. Только Path Tracing. Применяется сразу.'},
 'tr': {'tooltip_cr01_ambientocclusion': "Oyunun ortam kapatması. Kapalı FPS'i artırabilir. Yalnızca Path Tracing. "
                                         'Hemen uygulanır.',
        'tooltip_cr01_aocontrast': 'Ortam kapatmanın kontrastı. Görsel tercih; kanıtlanmış FPS kazancı yok. Yalnızca '
                                   'Path Tracing. Hemen uygulanır.',
        'tooltip_cr01_aoradius': 'Ortam kapatmanın ne kadar uzağa ulaştığı. Görsel tercih; kanıtlanmış FPS kazancı '
                                 'yok. Yalnızca Path Tracing. Hemen uygulanır.',
        'tooltip_cr01_aostrength': 'Ortam kapatmanın ne kadar koyu göründüğü. Görsel tercih; kanıtlanmış FPS kazancı '
                                   'yok. Yalnızca Path Tracing. Hemen uygulanır.',
        'tooltip_cr01_cpu_distant_trees': 'Uzak ağaç billboardlarını optimize eder. PT: Billboards açıkken Path '
                                          "Tracing'de duraklatılır. Açık FPS'i artırabilir. Normal Ray Tracing ve Path "
                                          'Tracing. Yeni yüklenen alanlara uygulanır.',
        'tooltip_cr01_cpu_fast_bone_reads': 'Karakter ve yaratıkların animasyon kemiklerinin CPU tarafından okunmasını '
                                            "hızlandırır; animasyon aynı görünür. Açık FPS'i artırabilir. Hemen "
                                            'uygulanır.',
        'tooltip_cr01_cpu_physics_lod': 'Fizik ayrıntısı için CPU işini azaltır; uzak nesneler ve kumaşlar daha az '
                                        "ayrıntılı hareket edebilir. Açık FPS'i artırabilir. Hemen uygulanır.",
        'tooltip_cr01_geometrybounces': "Işığın yüzeylerden kaç kez sektiği. Düşük değerler FPS'i artırabilir. "
                                        'Yalnızca Path Tracing. Hemen uygulanır.',
        'tooltip_cr01_grassday': 'Gündüz çimen parlaklığı. Görsel tercih; kanıtlanmış FPS kazancı yok. Yalnızca Path '
                                 'Tracing. Hemen uygulanır.',
        'tooltip_cr01_grassnight': 'Gece çimen parlaklığı. Görsel tercih; kanıtlanmış FPS kazancı yok. Yalnızca Path '
                                   'Tracing. Hemen uygulanır.',
        'tooltip_cr01_hairbounces': "Işığın saçın içinde kaç kez sektiği. Düşük değerler FPS'i artırabilir. Yalnızca "
                                    'Path Tracing. Hemen uygulanır.',
        'tooltip_cr01_indirectlight': 'Dolaylı ışığın parlaklığı (1.00 = nötr). Görsel tercih; kanıtlanmış FPS kazancı '
                                      'yok. Yalnızca Path Tracing. Hemen uygulanır.',
        'tooltip_cr01_pt_scene_range': 'Etrafındaki ışın izlenen alanı küçültür (oyun: 400 m); uzak yansımalar, '
                                       "dolaylı ışık ve gölgeler azalabilir. Düşük değerler FPS'i artırabilir. "
                                       'Yalnızca Path Tracing. Hemen uygulanır.',
        'tooltip_cr01_ptbillboards': "Uzak ağaç billboardlarının gölgeleri. Açık, CPU: Distant Trees'i duraklatır. "
                                     "Kapalı FPS'i artırabilir. Yalnızca Path Tracing. Yeni yüklenen alanlara "
                                     'uygulanır.',
        'tooltip_cr01_ptshadows': "Işın izlemeli gölgeler; Kapalı gölge ayrıntısını kaybeder. Kapalı FPS'i "
                                  'artırabilir. Yalnızca Path Tracing. Hemen uygulanır.',
        'tooltip_cr01_rt_perf_ao': "Temas gölgesi ön izlemesinin kalitesi. Düşük değerler FPS'i artırabilir. Yalnızca "
                                   "normal Ray Tracing. Hemen uygulanır. Ray-Traced Optimizations'ın Auto olması "
                                   'gerekir.',
        'tooltip_cr01_rt_perf_budget': 'Kare başına aydınlatma işi; düşük bütçe ışığı geciktirebilir veya çimeni '
                                       "soldurabilir. Düşük değerler FPS'i artırabilir. Yalnızca normal Ray Tracing. "
                                       "Hemen uygulanır. Ray-Traced Optimizations'ın Auto olması gerekir.",
        'tooltip_cr01_rt_perf_cap': 'Sınıflandırılan ışık sondası sayısını sınırlar; düşük sınırlar aydınlatma '
                                    "güncellemelerini geciktirebilir. Düşük değerler FPS'i artırabilir. Yalnızca "
                                    "normal Ray Tracing. Hemen uygulanır. Ray-Traced Optimizations'ın Auto olması "
                                    'gerekir.',
        'tooltip_cr01_rt_perf_global_shading_limit': 'Küresel ızgarada kare başına gölgelenen sondalar; düşük sınırlar '
                                                     "aydınlatmayı geciktirebilir. Düşük değerler FPS'i artırabilir. "
                                                     'Yalnızca normal Ray Tracing. Hemen uygulanır. Ray-Traced '
                                                     "Optimizations'ın Auto olması gerekir.",
        'tooltip_cr01_rt_perf_grid1_shading_limit': 'İkincil ızgarada kare başına gölgelenen sondalar; düşük sınırlar '
                                                    "aydınlatmayı geciktirebilir. Düşük değerler FPS'i artırabilir. "
                                                    'Yalnızca normal Ray Tracing. Hemen uygulanır. Ray-Traced '
                                                    "Optimizations'ın Auto olması gerekir.",
        'tooltip_cr01_rt_perf_primary': 'Küresel aydınlatmanın birincil sınıflandırma ışınları; daha az ışın '
                                        "aydınlatmayı değiştirebilir. Düşük değerler FPS'i artırabilir. Yalnızca "
                                        "normal Ray Tracing. Hemen uygulanır. Ray-Traced Optimizations'ın Auto olması "
                                        'gerekir.',
        'tooltip_cr01_rt_perf_secondary': 'Küresel aydınlatmanın ikincil sınıflandırma ışınları; daha az ışın '
                                          "aydınlatmayı değiştirebilir. Düşük değerler FPS'i artırabilir. Yalnızca "
                                          "normal Ray Tracing. Hemen uygulanır. Ray-Traced Optimizations'ın Auto "
                                          'olması gerekir.',
        'tooltip_cr01_rt_quality_ao': "Temas gölgesi ön izlemesinin kalitesi. Düşük değerler FPS'i artırabilir. "
                                      "Yalnızca normal Ray Tracing. Hemen uygulanır. Ray-Traced Optimizations'ın Auto "
                                      'olması gerekir.',
        'tooltip_cr01_rt_quality_budget': 'Kare başına aydınlatma işi; düşük bütçe ışığı geciktirebilir veya çimeni '
                                          "soldurabilir. Düşük değerler FPS'i artırabilir. Yalnızca normal Ray "
                                          "Tracing. Hemen uygulanır. Ray-Traced Optimizations'ın Auto olması gerekir.",
        'tooltip_cr01_rt_quality_cap': 'Sınıflandırılan ışık sondası sayısını sınırlar; düşük sınırlar aydınlatma '
                                       "güncellemelerini geciktirebilir. Düşük değerler FPS'i artırabilir. Yalnızca "
                                       "normal Ray Tracing. Hemen uygulanır. Ray-Traced Optimizations'ın Auto olması "
                                       'gerekir.',
        'tooltip_cr01_rt_quality_global_shading_limit': 'Küresel ızgarada kare başına gölgelenen sondalar; düşük '
                                                        "sınırlar aydınlatmayı geciktirebilir. Düşük değerler FPS'i "
                                                        'artırabilir. Yalnızca normal Ray Tracing. Hemen uygulanır. '
                                                        "Ray-Traced Optimizations'ın Auto olması gerekir.",
        'tooltip_cr01_rt_quality_grid1_shading_limit': 'İkincil ızgarada kare başına gölgelenen sondalar; düşük '
                                                       "sınırlar aydınlatmayı geciktirebilir. Düşük değerler FPS'i "
                                                       'artırabilir. Yalnızca normal Ray Tracing. Hemen uygulanır. '
                                                       "Ray-Traced Optimizations'ın Auto olması gerekir.",
        'tooltip_cr01_rt_quality_primary': 'Küresel aydınlatmanın birincil sınıflandırma ışınları; daha az ışın '
                                           "aydınlatmayı değiştirebilir. Düşük değerler FPS'i artırabilir. Yalnızca "
                                           "normal Ray Tracing. Hemen uygulanır. Ray-Traced Optimizations'ın Auto "
                                           'olması gerekir.',
        'tooltip_cr01_rt_quality_secondary': 'Küresel aydınlatmanın ikincil sınıflandırma ışınları; daha az ışın '
                                             "aydınlatmayı değiştirebilir. Düşük değerler FPS'i artırabilir. Yalnızca "
                                             "normal Ray Tracing. Hemen uygulanır. Ray-Traced Optimizations'ın Auto "
                                             'olması gerekir.',
        'tooltip_cr01_rt_refl_local_lights': 'Kapalı, yansımaları aydınlatırken lambaları, meşaleleri ve ateşleri yok '
                                             "sayar; aydınlık alanlar yansımalarda daha koyu görünür. Kapalı FPS'i "
                                             'artırabilir. Yalnızca normal Ray Tracing. Hemen uygulanır.',
        'tooltip_cr01_rt_shadow_foliage_detail': 'Gölge ışınları için daha kaba yaprak dokuları kullanır; yaprak '
                                                 "gölgeleri yumuşar. Daha az ayrıntı FPS'i artırabilir. Yalnızca "
                                                 'normal Ray Tracing. Hemen uygulanır.',
        'tooltip_cr01_rt_shadow_local_lights': 'Kaç yakın ışığın ışın izlemeli gölge düşürdüğü (oyun: 8); güneş '
                                               "gölgeleri değişmez. Düşük değerler FPS'i artırabilir. Yalnızca normal "
                                               'Ray Tracing. Hemen uygulanır.',
        'tooltip_cr01_skyimpact': 'Gökyüzü ışığının gücü (1.00 = nötr). Görsel tercih; kanıtlanmış FPS kazancı yok. '
                                  'Yalnızca Path Tracing. Hemen uygulanır.',
        'tooltip_cr01_specularcheckerboard': 'Bazı yansımalar için dama örneklemesi; Açık biraz ayrıntı '
                                             "kaybettirebilir. Açık FPS'i artırabilir. Yalnızca Path Tracing. Hemen "
                                             'uygulanır.',
        'tooltip_cr01_specularscreenspace': 'Bazı yansımalar için ekran alanı desteği; Kapalı ayrıntı kaybettirebilir. '
                                            "Kapalı FPS'i artırabilir. Yalnızca Path Tracing. Hemen uygulanır.",
        'tooltip_cr01_sunimpact': 'Güneş ışığının gücü (1.00 = nötr). Görsel tercih; kanıtlanmış FPS kazancı yok. '
                                  'Yalnızca Path Tracing. Hemen uygulanır.'},
 'ua': {'tooltip_cr01_ambientocclusion': 'Фонове затінення гри. Вимк. може підвищити FPS. Лише Path Tracing. '
                                         'Застосовується одразу.',
        'tooltip_cr01_aocontrast': 'Контраст фонового затінення. Справа смаку; підтвердженого приросту FPS немає. Лише '
                                   'Path Tracing. Застосовується одразу.',
        'tooltip_cr01_aoradius': 'Наскільки далеко сягає фонове затінення. Справа смаку; підтвердженого приросту FPS '
                                 'немає. Лише Path Tracing. Застосовується одразу.',
        'tooltip_cr01_aostrength': 'Наскільки темним виглядає фонове затінення. Справа смаку; підтвердженого приросту '
                                   'FPS немає. Лише Path Tracing. Застосовується одразу.',
        'tooltip_cr01_cpu_distant_trees': 'Оптимізує білборди далеких дерев. У Path Tracing призупиняється, поки '
                                          'увімкнено PT: Billboards. Увімк. може підвищити FPS. Звичайне трасування '
                                          'променів і Path Tracing. Застосовується до щойно завантажених областей.',
        'tooltip_cr01_cpu_fast_bone_reads': 'Пришвидшує читання процесором кісток анімації персонажів і істот; '
                                            'анімація виглядає так само. Увімк. може підвищити FPS. Застосовується '
                                            'одразу.',
        'tooltip_cr01_cpu_physics_lod': "Зменшує навантаження процесора на деталі фізики; далекі об'єкти й тканина "
                                        'можуть рухатися менш детально. Увімк. може підвищити FPS. Застосовується '
                                        'одразу.',
        'tooltip_cr01_geometrybounces': 'Скільки разів світло відбивається від поверхонь. Менші значення можуть '
                                        'підвищити FPS. Лише Path Tracing. Застосовується одразу.',
        'tooltip_cr01_grassday': 'Яскравість трави вдень. Справа смаку; підтвердженого приросту FPS немає. Лише Path '
                                 'Tracing. Застосовується одразу.',
        'tooltip_cr01_grassnight': 'Яскравість трави вночі. Справа смаку; підтвердженого приросту FPS немає. Лише Path '
                                   'Tracing. Застосовується одразу.',
        'tooltip_cr01_hairbounces': 'Скільки разів світло відбивається всередині волосся. Менші значення можуть '
                                    'підвищити FPS. Лише Path Tracing. Застосовується одразу.',
        'tooltip_cr01_indirectlight': 'Яскравість непрямого світла (1.00 = нейтрально). Справа смаку; підтвердженого '
                                      'приросту FPS немає. Лише Path Tracing. Застосовується одразу.',
        'tooltip_cr01_pt_scene_range': 'Зменшує область трасування навколо вас (гра: 400 м); далекі відображення, '
                                       'непряме світло й тіні можуть послабшати. Менші значення можуть підвищити FPS. '
                                       'Лише Path Tracing. Застосовується одразу.',
        'tooltip_cr01_ptbillboards': 'Тіні білбордів далеких дерев. Увімк. призупиняє CPU: Distant Trees. Вимк. може '
                                     'підвищити FPS. Лише Path Tracing. Застосовується до щойно завантажених областей.',
        'tooltip_cr01_ptshadows': 'Тіні з трасуванням променів; Вимк. втрачає деталізацію тіней. Вимк. може підвищити '
                                  'FPS. Лише Path Tracing. Застосовується одразу.',
        'tooltip_cr01_rt_perf_ao': 'Якість попереднього трасування контактних тіней. Менші значення можуть підвищити '
                                   'FPS. Лише звичайне трасування променів. Застосовується одразу. Потрібно Ray-Traced '
                                   'Optimizations у режимі Auto.',
        'tooltip_cr01_rt_perf_budget': 'Робота освітлення за кадр; низький бюджет може затримувати світло або робити '
                                       'траву блідою. Менші значення можуть підвищити FPS. Лише звичайне трасування '
                                       'променів. Застосовується одразу. Потрібно Ray-Traced Optimizations у режимі '
                                       'Auto.',
        'tooltip_cr01_rt_perf_cap': 'Обмежує кількість класифікованих світлових зондів; низькі межі можуть затримувати '
                                    'оновлення освітлення. Менші значення можуть підвищити FPS. Лише звичайне '
                                    'трасування променів. Застосовується одразу. Потрібно Ray-Traced Optimizations у '
                                    'режимі Auto.',
        'tooltip_cr01_rt_perf_global_shading_limit': 'Зонди глобальної сітки, що затінюються за кадр; низькі межі '
                                                     'можуть затримувати оновлення освітлення. Менші значення можуть '
                                                     'підвищити FPS. Лише звичайне трасування променів. Застосовується '
                                                     'одразу. Потрібно Ray-Traced Optimizations у режимі Auto.',
        'tooltip_cr01_rt_perf_grid1_shading_limit': 'Зонди вторинної сітки, що затінюються за кадр; низькі межі можуть '
                                                    'затримувати оновлення освітлення. Менші значення можуть підвищити '
                                                    'FPS. Лише звичайне трасування променів. Застосовується одразу. '
                                                    'Потрібно Ray-Traced Optimizations у режимі Auto.',
        'tooltip_cr01_rt_perf_primary': 'Первинні промені класифікації глобального освітлення; менше променів може '
                                        'змінити освітлення. Менші значення можуть підвищити FPS. Лише звичайне '
                                        'трасування променів. Застосовується одразу. Потрібно Ray-Traced Optimizations '
                                        'у режимі Auto.',
        'tooltip_cr01_rt_perf_secondary': 'Вторинні промені класифікації глобального освітлення; менше променів може '
                                          'змінити освітлення. Менші значення можуть підвищити FPS. Лише звичайне '
                                          'трасування променів. Застосовується одразу. Потрібно Ray-Traced '
                                          'Optimizations у режимі Auto.',
        'tooltip_cr01_rt_quality_ao': 'Якість попереднього трасування контактних тіней. Менші значення можуть '
                                      'підвищити FPS. Лише звичайне трасування променів. Застосовується одразу. '
                                      'Потрібно Ray-Traced Optimizations у режимі Auto.',
        'tooltip_cr01_rt_quality_budget': 'Робота освітлення за кадр; низький бюджет може затримувати світло або '
                                          'робити траву блідою. Менші значення можуть підвищити FPS. Лише звичайне '
                                          'трасування променів. Застосовується одразу. Потрібно Ray-Traced '
                                          'Optimizations у режимі Auto.',
        'tooltip_cr01_rt_quality_cap': 'Обмежує кількість класифікованих світлових зондів; низькі межі можуть '
                                       'затримувати оновлення освітлення. Менші значення можуть підвищити FPS. Лише '
                                       'звичайне трасування променів. Застосовується одразу. Потрібно Ray-Traced '
                                       'Optimizations у режимі Auto.',
        'tooltip_cr01_rt_quality_global_shading_limit': 'Зонди глобальної сітки, що затінюються за кадр; низькі межі '
                                                        'можуть затримувати оновлення освітлення. Менші значення '
                                                        'можуть підвищити FPS. Лише звичайне трасування променів. '
                                                        'Застосовується одразу. Потрібно Ray-Traced Optimizations у '
                                                        'режимі Auto.',
        'tooltip_cr01_rt_quality_grid1_shading_limit': 'Зонди вторинної сітки, що затінюються за кадр; низькі межі '
                                                       'можуть затримувати оновлення освітлення. Менші значення можуть '
                                                       'підвищити FPS. Лише звичайне трасування променів. '
                                                       'Застосовується одразу. Потрібно Ray-Traced Optimizations у '
                                                       'режимі Auto.',
        'tooltip_cr01_rt_quality_primary': 'Первинні промені класифікації глобального освітлення; менше променів може '
                                           'змінити освітлення. Менші значення можуть підвищити FPS. Лише звичайне '
                                           'трасування променів. Застосовується одразу. Потрібно Ray-Traced '
                                           'Optimizations у режимі Auto.',
        'tooltip_cr01_rt_quality_secondary': 'Вторинні промені класифікації глобального освітлення; менше променів '
                                             'може змінити освітлення. Менші значення можуть підвищити FPS. Лише '
                                             'звичайне трасування променів. Застосовується одразу. Потрібно Ray-Traced '
                                             'Optimizations у режимі Auto.',
        'tooltip_cr01_rt_refl_local_lights': 'Вимк. не враховує лампи, смолоскипи й вогнища під час освітлення '
                                             'відображень; освітлені місця в них темніші. Вимк. може підвищити FPS. '
                                             'Лише звичайне трасування променів. Застосовується одразу.',
        'tooltip_cr01_rt_shadow_foliage_detail': 'Використовує грубіші текстури листя для тіньових променів; тіні '
                                                 "листя стають м'якшими. Менша деталізація може підвищити FPS. Лише "
                                                 'звичайне трасування променів. Застосовується одразу.',
        'tooltip_cr01_rt_shadow_local_lights': 'Скільки близьких джерел світла відкидають тіні з трасуванням (гра: 8); '
                                               'тіні від сонця не змінюються. Менші значення можуть підвищити FPS. '
                                               'Лише звичайне трасування променів. Застосовується одразу.',
        'tooltip_cr01_skyimpact': 'Сила світла неба (1.00 = нейтрально). Справа смаку; підтвердженого приросту FPS '
                                  'немає. Лише Path Tracing. Застосовується одразу.',
        'tooltip_cr01_specularcheckerboard': 'Шахова вибірка для частини відображень; Увімк. може трохи втратити '
                                             'деталі. Увімк. може підвищити FPS. Лише Path Tracing. Застосовується '
                                             'одразу.',
        'tooltip_cr01_specularscreenspace': 'Допомога в екранному просторі для частини відображень; Вимк. може '
                                            'втратити деталі. Вимк. може підвищити FPS. Лише Path Tracing. '
                                            'Застосовується одразу.',
        'tooltip_cr01_sunimpact': 'Сила сонячного світла (1.00 = нейтрально). Справа смаку; підтвердженого приросту '
                                  'FPS немає. Лише Path Tracing. Застосовується одразу.'},
 'zh': {'tooltip_cr01_ambientocclusion': '遊戲本身的環境光遮蔽。關閉可能提升 FPS。僅限 Path Tracing。立即生效。',
        'tooltip_cr01_aocontrast': '環境光遮蔽的對比度。屬於視覺偏好，沒有已確認的 FPS 效益。僅限 Path Tracing。立即生效。',
        'tooltip_cr01_aoradius': '環境光遮蔽的影響範圍。屬於視覺偏好，沒有已確認的 FPS 效益。僅限 Path Tracing。立即生效。',
        'tooltip_cr01_aostrength': '環境光遮蔽的暗度。屬於視覺偏好，沒有已確認的 FPS 效益。僅限 Path Tracing。立即生效。',
        'tooltip_cr01_cpu_distant_trees': '最佳化遠處樹木的看板。PT: Billboards 開啟時，在 Path Tracing 中暫停。開啟可能提升 FPS。一般 Ray Tracing '
                                          '與 Path Tracing。對新載入的區域生效。',
        'tooltip_cr01_cpu_fast_bone_reads': '加快 CPU 讀取角色與生物動畫骨骼的速度；動畫效果不變。開啟可能提升 FPS。立即生效。',
        'tooltip_cr01_cpu_physics_lod': '減少物理細節的 CPU 工作；遠處物件與布料的動作可能較不精細。開啟可能提升 FPS。立即生效。',
        'tooltip_cr01_geometrybounces': '光線在表面之間反彈的次數。降低數值可能提升 FPS。僅限 Path Tracing。立即生效。',
        'tooltip_cr01_grassday': '白天的草地亮度。屬於視覺偏好，沒有已確認的 FPS 效益。僅限 Path Tracing。立即生效。',
        'tooltip_cr01_grassnight': '夜晚的草地亮度。屬於視覺偏好，沒有已確認的 FPS 效益。僅限 Path Tracing。立即生效。',
        'tooltip_cr01_hairbounces': '光線在頭髮內部反彈的次數。降低數值可能提升 FPS。僅限 Path Tracing。立即生效。',
        'tooltip_cr01_indirectlight': '間接光亮度（1.00 = 中性）。屬於視覺偏好，沒有已確認的 FPS 效益。僅限 Path Tracing。立即生效。',
        'tooltip_cr01_pt_scene_range': '縮小你周圍的光線追蹤範圍（遊戲預設：400 m）；遠處的反射、間接光與陰影可能減弱。降低數值可能提升 FPS。僅限 Path Tracing。立即生效。',
        'tooltip_cr01_ptbillboards': '遠處樹木看板的陰影。開啟會暫停 CPU: Distant Trees。關閉可能提升 FPS。僅限 Path Tracing。對新載入的區域生效。',
        'tooltip_cr01_ptshadows': '光線追蹤陰影；關閉會失去陰影細節。關閉可能提升 FPS。僅限 Path Tracing。立即生效。',
        'tooltip_cr01_rt_perf_ao': '接觸陰影預追蹤的品質。降低數值可能提升 FPS。僅限一般 Ray Tracing。立即生效。需要將 Ray-Traced Optimizations 設為 '
                                   'Auto。',
        'tooltip_cr01_rt_perf_budget': '每幀的光照工作量；過低可能延遲光照或使草地變淡。降低數值可能提升 FPS。僅限一般 Ray Tracing。立即生效。需要將 Ray-Traced '
                                       'Optimizations 設為 Auto。',
        'tooltip_cr01_rt_perf_cap': '限制被分類的光照探針數量；過低可能延遲光照更新。降低數值可能提升 FPS。僅限一般 Ray Tracing。立即生效。需要將 Ray-Traced '
                                    'Optimizations 設為 Auto。',
        'tooltip_cr01_rt_perf_global_shading_limit': '全域網格每幀著色的探針數；過低可能延遲光照更新。降低數值可能提升 FPS。僅限一般 Ray Tracing。立即生效。需要將 '
                                                     'Ray-Traced Optimizations 設為 Auto。',
        'tooltip_cr01_rt_perf_grid1_shading_limit': '次級網格每幀著色的探針數；過低可能延遲光照更新。降低數值可能提升 FPS。僅限一般 Ray Tracing。立即生效。需要將 '
                                                    'Ray-Traced Optimizations 設為 Auto。',
        'tooltip_cr01_rt_perf_primary': '全域光照的主要分類光線；減少光線可能改變光照。降低數值可能提升 FPS。僅限一般 Ray Tracing。立即生效。需要將 Ray-Traced '
                                        'Optimizations 設為 Auto。',
        'tooltip_cr01_rt_perf_secondary': '全域光照的次要分類光線；減少光線可能改變光照。降低數值可能提升 FPS。僅限一般 Ray Tracing。立即生效。需要將 Ray-Traced '
                                          'Optimizations 設為 Auto。',
        'tooltip_cr01_rt_quality_ao': '接觸陰影預追蹤的品質。降低數值可能提升 FPS。僅限一般 Ray Tracing。立即生效。需要將 Ray-Traced Optimizations 設為 '
                                      'Auto。',
        'tooltip_cr01_rt_quality_budget': '每幀的光照工作量；過低可能延遲光照或使草地變淡。降低數值可能提升 FPS。僅限一般 Ray Tracing。立即生效。需要將 Ray-Traced '
                                          'Optimizations 設為 Auto。',
        'tooltip_cr01_rt_quality_cap': '限制被分類的光照探針數量；過低可能延遲光照更新。降低數值可能提升 FPS。僅限一般 Ray Tracing。立即生效。需要將 Ray-Traced '
                                       'Optimizations 設為 Auto。',
        'tooltip_cr01_rt_quality_global_shading_limit': '全域網格每幀著色的探針數；過低可能延遲光照更新。降低數值可能提升 FPS。僅限一般 Ray '
                                                        'Tracing。立即生效。需要將 Ray-Traced Optimizations 設為 Auto。',
        'tooltip_cr01_rt_quality_grid1_shading_limit': '次級網格每幀著色的探針數；過低可能延遲光照更新。降低數值可能提升 FPS。僅限一般 Ray Tracing。立即生效。需要將 '
                                                       'Ray-Traced Optimizations 設為 Auto。',
        'tooltip_cr01_rt_quality_primary': '全域光照的主要分類光線；減少光線可能改變光照。降低數值可能提升 FPS。僅限一般 Ray Tracing。立即生效。需要將 Ray-Traced '
                                           'Optimizations 設為 Auto。',
        'tooltip_cr01_rt_quality_secondary': '全域光照的次要分類光線；減少光線可能改變光照。降低數值可能提升 FPS。僅限一般 Ray Tracing。立即生效。需要將 Ray-Traced '
                                             'Optimizations 設為 Auto。',
        'tooltip_cr01_rt_refl_local_lights': '關閉後照亮反射時會忽略燈、火把與火焰；被照亮的區域在反射中較暗。關閉可能提升 FPS。僅限一般 Ray Tracing。立即生效。',
        'tooltip_cr01_rt_shadow_foliage_detail': '陰影光線使用較粗糙的樹葉貼圖；樹葉陰影變得較柔和。降低細節可能提升 FPS。僅限一般 Ray Tracing。立即生效。',
        'tooltip_cr01_rt_shadow_local_lights': '投射光線追蹤陰影的附近光源數量（遊戲預設：8）；太陽陰影不受影響。降低數值可能提升 FPS。僅限一般 Ray Tracing。立即生效。',
        'tooltip_cr01_skyimpact': '天空光強度（1.00 = 中性）。屬於視覺偏好，沒有已確認的 FPS 效益。僅限 Path Tracing。立即生效。',
        'tooltip_cr01_specularcheckerboard': '部分反射的棋盤格取樣；開啟可能失去少量細節。開啟可能提升 FPS。僅限 Path Tracing。立即生效。',
        'tooltip_cr01_specularscreenspace': '部分反射的螢幕空間輔助；關閉可能失去反射細節。關閉可能提升 FPS。僅限 Path Tracing。立即生效。',
        'tooltip_cr01_sunimpact': '陽光強度（1.00 = 中性）。屬於視覺偏好，沒有已確認的 FPS 效益。僅限 Path Tracing。立即生效。'}}
