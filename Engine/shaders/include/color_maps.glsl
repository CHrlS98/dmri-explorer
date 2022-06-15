/*Color Maps*/

const vec3 smooth_cool_warm[32] = vec3[32]
(
    vec3(0.229999503869523,	0.298998934049376,	0.754000138575591),
    vec3(0.267668055063561,	0.355096960116671,	0.803217559723499),
    vec3(0.30661746745018,	0.410187724830986,	0.847846340564029),
    vec3(0.346943858303143,	0.463956977963287,	0.887413741243394),
    vec3(0.388607969573877,	0.516024373378667,	0.921506648307719),
    vec3(0.431460910661207,	0.565979514265992,	0.949775526699291),
    vec3(0.475265270078664,	0.61340182837898,	0.971937578406071),
    vec3(0.519713444928311,	0.657872992657594,	0.98777904624352),
    vec3(0.564443706947279,	0.698985588450434,	0.997156621471119),
    vec3(0.609054191814504,	0.736349680395601,	0.999997931665274),
    vec3(0.653114974941615,	0.769598146887948,	0.996301100970284),
    vec3(0.696178435134982,	0.798391183276044,	0.986133388526255),
    vec3(0.737788131907965,	0.82242019493211,	0.969628922147414),
    vec3(0.777486421062903,	0.841411188924205,	0.946985552419257),
    vec3(0.814821013908372,	0.855127711780773,	0.91846085596996),
    vec3(0.849350657956925,	0.863373343326275,	0.884367313521273),
    vec3(0.882920335710997,	0.857543043292736,	0.842647209942362),
    vec3(0.913047805357375,	0.837743982160493,	0.795652095529871),
    vec3(0.936758485448242,	0.812632657656555,	0.746941192975795),
    vec3(0.954102406934724,	0.782418365796981,	0.697008842718602),
    vec3(0.965128523848557,	0.747340307025475,	0.646332707923289),
    vec3(0.96989710604693,	0.707660328102259,	0.595369373866591),
    vec3(0.96848823076525,	0.663653169412791,	0.544550565937067),
    vec3(0.96100769694686,	0.615592697768693,	0.494280031015227),
    vec3(0.947591145584413,	0.563731210880007,	0.444931132795449),
    vec3(0.928406861903564,	0.508265852857986,	0.396845226924106),
    vec3(0.903657553904819,	0.449278895564548,	0.350330912178581),
    vec3(0.873581291745818,	0.386619057548273,	0.305664308421561),
    vec3(0.838451724419193,	0.319629211858138,	0.263090606021473),
    vec3(0.798577647985909,	0.246378185858256,	0.222827290298822),
    vec3(0.754301974268325,	0.160555336453242,	0.185069708729985),
    vec3(0.706000135911705,	0.015991824033981,	0.1500000719222)
);

const vec3 bent_cool_warm[32] = vec3[32]
(
    vec3(0.229999503869522,	0.298998934049376,	0.754000138575591),
    vec3(0.261897173115164,	0.343574659088609,	0.774690567385777),
    vec3(0.296108000825267,	0.387375989626998,	0.793877789635088),
    vec3(0.332480010388889,	0.430662378848291,	0.81163047320303),
    vec3(0.370904626079103,	0.473584677956896,	0.828024053635061),
    vec3(0.411306934300608,	0.516230566370244,	0.84314028043576),
    vec3(0.453637617580722,	0.558648628358969,	0.857066828841311),
    vec3(0.497866557190087,	0.600862052495685,	0.869896973972842),
    vec3(0.543977829876652,	0.642876874895762,	0.881729324663964),
    vec3(0.591965791578736,	0.684687172252004,	0.892667614075972),
    vec3(0.641831986031125,	0.726278464307926,	0.902820543511705),
    vec3(0.693582675748417,	0.767630022884055,	0.912301674645668),
    vec3(0.747226845612232,	0.808716491434785,	0.921229363733007),
    vec3(0.802774570276869,	0.849509058534099,	0.929726729283683),
    vec3(0.860235666622719,	0.889976336933336,	0.937921642256684),
    vec3(0.919618573810807,	0.930085045478758,	0.945946725146929),
    vec3(0.944985651198613,	0.925687015309531,	0.914427955619266),
    vec3(0.934591247444014,	0.876892412107381,	0.845044207878051),
    vec3(0.923680509339917,	0.827852703830501,	0.777928130077826),
    vec3(0.91213839825554,	0.778552184390633,	0.713176107498793),
    vec3(0.899874113228032,	0.728960380164992,	0.650878301493416),
    vec3(0.886817447145052,	0.679026925071541,	0.591119051820929),
    vec3(0.872915369865288,	0.628673806303087,	0.533977493444033),
    vec3(0.858128892540186,	0.577783209091799,	0.479528454166354),
    vec3(0.842430239602922,	0.526177626875995,	0.427843723211146),
    vec3(0.825800336402574,	0.473585527961961,	0.378993810804653),
    vec3(0.808226608921504,	0.419577883418097,	0.333050352175209),
    vec3(0.789701085452629,	0.363439599709505,	0.290089334268046),
    vec3(0.77021878701673,	0.303873404762745,	0.250195305045485),
    vec3(0.749776392596696,	0.238169744593455,	0.213466574080148),
    vec3(0.728371166287474,	0.158891958114337,	0.180020921507554),
    vec3(0.706000135911705,	0.01599182403398,	0.1500000719222)
);

const vec3 viridis[32] = vec3[32]
(
    vec3(0.267003985321379,	0.00487256571458,	0.329415068552478),
    vec3(0.277228997986469,	0.051716984093232,	0.376949910459682),
    vec3(0.282479689880534,	0.09733496394981,	0.41951057504893),
    vec3(0.282711275772715,	0.139317688354087,	0.456197067992232),
    vec3(0.278092634974176,	0.179895882776344,	0.486377420515705),
    vec3(0.269137786704195,	0.219429659297187,	0.509890869791121),
    vec3(0.256733531668467,	0.257754383167119,	0.527183780112339),
    vec3(0.242031460641851,	0.29464381588309,	0.539209023851904),
    vec3(0.226243755775175,	0.32998932861686,	0.5471628256332),
    vec3(0.210443168206333,	0.363856060641545,	0.552221276249024),
    vec3(0.195412485534445,	0.396435844353248,	0.555350926415028),
    vec3(0.181477324689768,	0.428017313726078,	0.557198853980301),
    vec3(0.16857422848523,  0.458905236814897,	0.558067330312246),
    vec3(0.156365948599481,	0.489384597876847,	0.557941172397524),
    vec3(0.144535293616217,	0.519685615329712,	0.55652766324792),
    vec3(0.133249551891658,	0.549958246842863,	0.553339219207683),
    vec3(0.123833067074232,	0.580259243259126,	0.547771637159412),
    vec3(0.119442112014129,	0.610546221244076,	0.539182010093552),
    vec3(0.124881901507986,	0.640695014465309,	0.526954942132968),
    vec3(0.144277738124196,	0.670499732330007,	0.510554715910401),
    vec3(0.178281445015228,	0.699705646022725,	0.489567133611302),
    vec3(0.22479743911, 	0.728014409923207,	0.463677887436587),
    vec3(0.281243457587794,	0.755097766154938,	0.432683203541095),
    vec3(0.345693488840013,	0.780604756965945,	0.396465688913023),
    vec3(0.416705431982845,	0.804185309816143,	0.355029984661802),
    vec3(0.493228828580062,	0.825506230941773,	0.308497656879776),
    vec3(0.574270238040451,	0.844288831352697,	0.25725770433235),
    vec3(0.658654029188311,	0.860389968259754,	0.202434460987441),
    vec3(0.744780537450654,	0.873933018329349,	0.14754782129648),
    vec3(0.830610047311472,	0.885437755008298,	0.104273580133882),
    vec3(0.914002409532199,	0.895811263860497,	0.100134277716998),
    vec3(0.99324814893356,  0.906154763420806,	0.143935943669684)
);

const vec3 plasma[32] = vec3[32]
(
    vec3(0.050382053470599,	0.029801736499742,	0.527975101049518),
    vec3(0.134263095956425,	0.022129041464033,	0.564086943874442),
    vec3(0.19658429522455,	0.018148723085373,	0.591699907010603),
    vec3(0.252508903744709,	0.014059729837062,	0.614596052572696),
    vec3(0.305597246846751,	0.008962932182813,	0.633493656287653),
    vec3(0.357161342424505,	0.003722446287555,	0.64799537276208),
    vec3(0.40766452865892,	0.00063542187127,	0.657244390906515),
    vec3(0.45713493296072,	0.003219966498149,	0.660291088653498),
    vec3(0.505338779334359,	0.015879960361365,	0.656373803832435),
    vec3(0.551894068905485,	0.043267350966154,	0.645217958234789),
    vec3(0.596370855317254,	0.078310546152515,	0.627252928317216),
    vec3(0.638408558026694,	0.114225740031185,	0.603686083070476),
    vec3(0.677817333874352,	0.150564237726243,	0.576205185917591),
    vec3(0.714604238385043,	0.187011213613401,	0.546574897373828),
    vec3(0.748939980627268,	0.223423831835724,	0.516239938838344),
    vec3(0.781069149977098,	0.259789608780162,	0.486138795941628),
    vec3(0.811236242283869,	0.296210417771778,	0.45670663787859),
    vec3(0.839614821321557,	0.33285651624084,	0.428012933217697),
    vec3(0.866280201095155,	0.369951912397464,	0.399908030028534),
    vec3(0.891185920886601,	0.407746777797116,	0.37215683516806),
    vec3(0.914179886198195,	0.446494219266261,	0.344520445409926),
    vec3(0.935012277718101,	0.486438292121575,	0.316826252519023),
    vec3(0.953361035955715,	0.527793036024391,	0.288993337404004),
    vec3(0.968852021999771,	0.570730380904922,	0.261068305515935),
    vec3(0.981089981012577,	0.615358169581107,	0.233246532554544),
    vec3(0.989650252591629,	0.661747758025893,	0.206018553736301),
    vec3(0.994066749767429,	0.709927390700005,	0.180482004740676),
    vec3(0.993816357899574,	0.75990211771125,	0.1588815644222),
    vec3(0.988319905382877,	0.811642117364466,	0.14503890140546),
    vec3(0.977050900222136,	0.865041805460105,	0.143105142180541),
    vec3(0.959823007609692,	0.919885356968148,	0.151329147688882),
    vec3(0.940015127878274,	0.975155785620538,	0.131325887773911)
);

const vec3 black_body[32] = vec3[32]
(
    vec3(0,	                0,              	0),
    vec3(0.088295941315832,	0.030877237446357,	0.017299100824261),
    vec3(0.136516564533188,	0.058703045701813,	0.034609274683041),
    vec3(0.183953173358167,	0.073031348078966,	0.051278430681639),
    vec3(0.234505777269526,	0.084187534672359,	0.064288530752967),
    vec3(0.286929808854981,	0.09421838495169,	0.074647568964532),
    vec3(0.340954275640275,	0.103150342433552,	0.083166189746154),
    vec3(0.396303488004765,	0.111020332434493,	0.091347784961288),
    vec3(0.452877649024806,	0.117814292639757,	0.099527606096225),
    vec3(0.510617206445444,	0.12349315704899,	0.107709512666186),
    vec3(0.569465699831873,	0.128001883973017,	0.115896708468964),
    vec3(0.629370765502709,	0.13126643742666,	0.124091875045229),
    vec3(0.690284292533846,	0.133188913189814,	0.132297272452977),
    vec3(0.726137522154784,	0.183416631080585,	0.12677946437298),
    vec3(0.758242260648654,	0.233262137948032,	0.117137304784621),
    vec3(0.790301059864388,	0.279149001228305,	0.104519154396313),
    vec3(0.822321479633414,	0.32282931091353,	0.087657620039036),
    vec3(0.854310321886797,	0.365181111979494,	0.063794223462689),
    vec3(0.886273726809331,	0.406705312446279,	0.025283520154189),
    vec3(0.89656246624484,	0.462802967876054,	0.035822541393613),
    vec3(0.902646986221922,	0.51836020562993,	0.057028330819568),
    vec3(0.907431440704957,	0.572008510444337,	0.07751537002526),
    vec3(0.910857530116435,	0.624333181330056,	0.097626113439732),
    vec3(0.912860427463949,	0.675721019856952,	0.117569585723736),
    vec3(0.913367581353788,	0.726440019827146,	0.13746156740881),
    vec3(0.912297256545023,	0.776682649933214,	0.15737138729393),
    vec3(0.909556728280631,	0.826591079544904,	0.177342781437844),
    vec3(0.905040015471748,	0.876272712858049,	0.197404326288731),
    vec3(0.925716214483183,	0.914803881675651,	0.349722558850904),
    vec3(0.965517070579819,	0.942108120352842,	0.579851881878355),
    vec3(0.991013729642934,	0.970448027480562,	0.791169850569277),
    vec3(1,                	1,              	1)
);

const vec3 inferno[32] = vec3[32]
(
    vec3(0.001461995581172,	0.000465991391911,	0.013866005775116),
    vec3(0.014574183301667,	0.011656264840479,	0.073830275000047),
    vec3(0.044498905459586,	0.029116239242626,	0.145204474280141),
    vec3(0.085624579940771,	0.04421609649874,	0.221732415708434),
    vec3(0.135148534668839,	0.046903282611846,	0.298903812870729),
    vec3(0.191262016007003,	0.039195890017257,	0.362232020997112),
    vec3(0.247330969897306,	0.037282617172547,	0.40120279418778),
    vec3(0.300892746118114,	0.048584344231213,	0.421473061954073),
    vec3(0.352822193460819,	0.066474865657373,	0.430772826919857),
    vec3(0.404095201720847,	0.085652524212783,	0.43317652187523),
    vec3(0.45525951586368,	0.104422635010854,	0.430330236001234),
    vec3(0.50651403211683,	0.122632482711627,	0.422776038580776),
    vec3(0.557817338373578,	0.140700063227475,	0.410586989483809),
    vec3(0.608931089391169,	0.159321266403542,	0.393737911103831),
    vec3(0.659445031183223,	0.179374593999117,	0.372280542494421),
    vec3(0.708784045991532,	0.201857697666751,	0.34646546360191),
    vec3(0.756245510545892,	0.227781546747388,	0.316747266566838),
    vec3(0.801022430085443,	0.258034860953672,	0.28377395969358),
    vec3(0.842275371686632,	0.293229431471329,	0.248278764412965),
    vec3(0.879217312210487,	0.333576791927403,	0.210931541250288),
    vec3(0.911236670448773,	0.378839873393796,	0.172100391360499),
    vec3(0.937920427556078,	0.42847614713637,	0.131750098006253),
    vec3(0.959041323406337,	0.481797279691003,	0.089671922495064),
    vec3(0.974467976960987,	0.538138942534478,	0.047498085410011),
    vec3(0.984122186388353,	0.596899570900953,	0.023699889114393),
    vec3(0.987891558047522,	0.657597147644721,	0.049598455557408),
    vec3(0.985647091751464,	0.719796009674981,	0.111167933613546),
    vec3(0.977365265620369,	0.783003534412184,	0.186911782687614),
    vec3(0.963797680239549,	0.846307154723048,	0.277334152708613),
    vec3(0.949298966312484,	0.907280261946158,	0.388527436363405),
    vec3(0.951251740825786,	0.959336844836301,	0.520521918975288),
    vec3(0.988362079921221,	0.998361647062055,	0.644924098280386)
);
