<?php
/*
 * convert from ggb (XML) to FOF (not yet completed)
 *
 */

// $xml=simplexml_load_string(gzuncompress(file_get_contents("/Users/yannis/test2.ggb")));
// print_r($xml);

if ($argc < 2) {
    echo "\nNo enough arguments, usage: php filterGEOGEBRAtoFOF.php input.ggb > output.p\n\n";
    exit;
}
elseif (!file_exists($argv[1])) {
    echo "\nFile ".$argv[1]." does not exist\n\n";
    exit;
}

$inputFile = $argv[1];
//$outputFile = fopen($argv[2], "w");

$zipfile = $inputFile;
$zip = zip_open($zipfile);
$ziparc = new ZipArchive;
$xml = new DOMDocument();

// begin writing to a buffer
//ob_start();

/*
 * Opens de Geogebra Zip file (.ggb) and extract the geogebra.xml out of it
 */

  if ($zip) {
    while ($zip_entry = zip_read($zip)) {
      $file = zip_entry_name($zip_entry);
      //echo "Name: " . $file . "<br />";

      if (strpos($file,'geogebra.xml') !== false) {
        if ($ziparc->open($zipfile) === TRUE) {
          $xml->loadXML($ziparc->getFromName($file));
#			print_r($qqch);
          $ziparc->close();
        } else {
          echo 'failed';
        }
      }
    }
    zip_close($zip);
  }

// Global variables
$IS_IN_FACT=array();
$TYPE=array();
$POINTS = array(); // array of points
$NUMPOINTS = 0; // Number of points
$LINES = array(); // associative array, id. line + id points (>2)
// array of circles
// [label,[center,radius,[point,point,...]]], e.g. ['c',['O',23,['A,']]]
// 
$CIRCLES = array();

/*
 * Add a relation
 */

// YH
function add_relation($lefttype,$left,$rel,$righttype,$right) {
//    print "$lefttype $left $rel $righttype $right\n";
}

// PQ
function addRelation($left,$right) {
    print "perp("."$left".","."$right".")\n";
}

// PQ
/*
 * Define the predicate perp(A,B,C,D) - orthogonal lines
 *
 * the line1 and line2 should be already exist and have associated a
 * list of at least two points
 */
function addOrthogonalLine($line1,$line2) {
    global $LINES;

    
    $varString ="perp(";
    // points of line1
    $points = $LINES[$line1];

    $auxInt = 0;
    foreach ($points as $key => $value){
        // only the first two points are needed
        if ($auxInt <= 1) {
            $varString .= $value.",";
        }
        $auxInt++;
    }
    $auxInt = 0;
    // points of line2
    $points = $LINES[$line2];
    foreach ($points as $key => $value){
        // only the first two points are needed and only one comma
        if ($auxInt < 1) {
            $varString .= $value.",";
        }
        elseif ($auxInt == 1) {
            $varString .= $value;
        }
        $auxInt++;
    }
    $varString .= ")\n";
    echo $varString;
//    var_dump($LINES);
}

// PQ
/*
 * Define the predicate para(A,B,C,D) - parallel lines
 *
 * the line1 and line2 should already exist and have associated a
 * list of at least two points
 */
function addParallelLines($line1,$line2) {
    global $LINES;

    
    
    $varString ="para(";

    // points of line1
    $points = $LINES[$line1];

    $auxInt = 0;
    foreach ($points as $key => $value){
        // only the first two points are needed
        if ($auxInt <= 1) {
            $varString .= $value.",";
        }
        $auxInt++;
    }
    $auxInt = 0;

    // points of line2
    $points = $LINES[$line2];
//  var_dump($LINES);
    foreach ($points as $key => $value){
        // only the first two points are needed and only one comma
        if ($auxInt < 1) {
            $varString .= $value.",";
        }
        elseif ($auxInt == 1) {
            $varString .= $value;
        }
        $auxInt++;
    }
    $varString .= ")\n";
    echo $varString;
//    var_dump($LINES);
}

/*
 * Add a point PQ
 */ 
function addPoint($label) { 

    global $TYPE; // YH

    global $POINTS;
    global $NUMPOINTS;
    
    if(!array_key_exists($label,$POINTS)){
        $POINTS[$label] = $label;
        $NUMPOINTS++;
    }
    
    $TYPE[$label]="POINT"; // YH
}

/*
 * Add a Point that belongs to a give object (line or circle)
 * <-- point (existing point)
 * <-- object (existing line or circle)
 * --> add the point to the object
 *  
 */
function addPointOnObject($point,$object) {
    
    global $LINES;
    global $CIRCLES;

    // The object should already exist and the point to

    // check if the element is a line or a circle
    if (array_key_exists("$object",$LINES)) {
        $typeObject = "line";
    }
    if (array_key_exists("$object",$CIRCLES)) {
        $typeObject = "circle"; 
    }

    switch ($typeObject) {
    case "line":
        $listpoints = $LINES[$object];  
        break;
    case "circle":
        $listpoints = $CIRCLES[$object]["Points"];
        break;
    default:
        echo "Its a UFO!!!\n";
    } 
    
    // adding a new point to the list of points of the object
    $listpoints["$point"] = "$point";

    // update the object (new list of points)
    switch ($typeObject) {
    case "line":
        $LINES[$object] = $listpoints;
        break;
    case "circle":
        $CIRCLES[$object]["Points"] = $listpoints;
        break;
    default:
        echo "Its a UFO!!!\n";
    } 
    

}




/*
 * Add a intersection point to two lines PQ
 */ 
function addIntersection($line1,$line2,$point) { 

global $TYPE; 
    global $LINES;
    global $POINTS;
    global $NUMPOINTS;
    
//    $POINTS[$NUMPOINTS] = $point;
//    $NUMPOINTS++;

    addPoint($point);

//    echo "========\n\t".$line1."\t".$line2."\t".$point."\n";
    
//    var_dump($LINES);

//    echo "Add an intersection lines";
    
    // points of line1
    $pointsLine1 = $LINES[$line1];
    // points of line2
    $pointsLine2 = $LINES[$line2];
    
    // adding a new point to the list of points of the two lines
    $pointsLine1["$point"] = "$point";
    $pointsLine2["$point"] = "$point";

    // update the lines (new list of points)
    $LINES[$line1] = $pointsLine1;
    $LINES[$line2] = $pointsLine2;

$TYPE[$point]="POINT";
}


/*
 * Add a intersection point to two lines PQ
 * 
 * -> midpoint, segmentAB
 * <- adding 'midpoint' to the list of points
 * <- adding the 'midpoint' to the points on the line that has 
 *    points 'pointA', 'pointB'
 * <- build the attribute "midpoint('midpoint','pointA','pointB')
 *
 * We have to check what are the points making segment 'segmentAB'
 * to add to 'midpoint' to the list of points of that line
 */ 
function addMidPoint($midpoint,$segmentAB) { 

global $TYPE; 
    global $LINES;
    global $POINTS;
    global $NUMPOINTS;
    
//    $POINTS[$NUMPOINTS] = $point;
//    $NUMPOINTS++;

    addPoint($midpoint);
    
    // begin building the attribute
    $varString = "midp(".$midpoint.",";

    // the line containing the points A and B already exist
    $inLine = False;

//    var_dump($LINES);

    // gets the poing from the segment

    $auxInt = 0;
    // points of line2
    $points = $LINES[$segmentAB];
    
    foreach ($points as $key => $value){
        // only the first two points are needed and only one comma
        if ($auxInt < 1) {
            $varString .= $value.",";
        }
        elseif ($auxInt == 1) {
            $varString .= $value;
        }
        $auxInt++;
    }
    
    $varString .= ")\n";

    echo $varString;

    // The new point must be added to the list

    $points["$midpoint"] = "$midpoint";

    $LINES[$segmentAB] = $points;

//    var_dump($LINES);
    
    /*
    reset($LINES);
    while ((list($key, $value) = each($LINES)) && (!$inLine)) {
        if (in_array($pointA, $value) && in_array($pointA, $value)) {
            $inLine = True;
            $idLine = $key;
        }
    }
    
    // if inLine is true the line exist, we have to add 'midpoint' to
    // it, if not we have to create the line first.
    
    if ($inLine) {
        // points of line
        $pointsLine = $LINES[$idLine];

        // adding a new point to the list of points of the two lines
        $pointsLine["$midpoint"] = "$midpoint";
        
        // update the lines (new list of points)
        $LINES[$idLine] = $pointsLine;
    }
    else {
        echo "line does not exist\n\n";
        $label = $pointA."".$pointB;
        addLine($label,$pointA,$pointB);
    }
    
    */
    $TYPE[$midpoint]="POINT";
}



/*
 * Add a point to a line ('inventing' a new point to that line)
 */
function defineOrthogonalLine($point,$line) {
    global $LINES;
    global $NUMPOINTS;

    $points = array();
    
    $auxNP=$NUMPOINTS;
    // to add a new point as 'label'+(numpoints+1)
    $auxNP++;
    $newPoint = $point.$auxNP;
    // for now I will add the new point to the list of points
    addPoint($newPoint);
    // creating the array of points
    $points["$point"] = "$point";
    $points["$newPoint"] = "$newPoint";
    // the points are associated to the line identifier
    $LINES["$line"] = $points;
//    var_dump($LINES);
}

/*
 * Define a line with a label and a point
 * -> line,point
 * <- line = point, newPoint
 *
 * Add a point to a line ('inventing' a new point to that line)
 */
function defineParallelLine($point,$line) {
    global $LINES;
    global $NUMPOINTS;

    $points = array();
    
    $auxNP=$NUMPOINTS;
    // new points = 'label'+(numpoints+1)
    $auxNP++;
    $newPoint = $point.$auxNP;

    // The new point is added to the list of points
    addPoint($newPoint);
    // creating the array of points
    $points["$point"] = "$point";
    $points["$newPoint"] = "$newPoint";
    // the points are associated to the line identifier
    $LINES["$line"] = $points;
}


/*
 * Add a Segment 
 * <-- label (name of segm.), left (point), right (point)
 * --> create segment 
 *     add two relations 'belongs_to' (left and right belongs to)
 *  
 */
// YH
function add_segment($label,$left,$right) {
    global $IS_IN_FACT,$TYPE;
//    print "CREATE SEGMENT ".$label."\n";
    add_relation("POINT",$left,"BELONGS_TO","SEGMENT",$label);
    add_relation("POINT",$right,"BELONGS_TO","SEGMENT",$label);
    $IS_IN_FACT["$left:$right"]=$label;
    $IS_IN_FACT["$right:$left"]=$label;
    $TYPE[$label]="SEGMENT";
}

// PQ
/*
 * the segment will not be echoed, but the segment will be added to
 * the lines array along side its points
 */
function addSegment($label,$left,$right) {
global $IS_IN_FACT,$TYPE;
    global $LINES;

    // If the points where already been created... there are created
    // again
    addPoint($left);
    addPoint($right);
    // creating the array of points
    $points["$left"] = "$left";
    $points["$right"] = "$right";
    echo "segment(".$left.",".$right.")\n";
    // create the line and its associated points
    $LINES["$label"] = $points;
$IS_IN_FACT["$left:$right"]=$label;
$IS_IN_FACT["$right:$left"]=$label;
$TYPE[$label]="SEGMENT";
}

/*
 * Add Line
 * <-- label (name of line)
 * --> create a line
 */
function add_line($label) {
global $TYPE;
//    print "CREATE LINE ".$label."\n";
$TYPE[$label]="LINE";
}

// PQ
/*
 * the line will not be echoed, but the line will be added to the
 * lines array along side its points
 */
function addLine($label,$left,$right) {
global $TYPE;
    global $LINES;

    // Adding the points to the list of points
    addPoint($left);
    addPoint($right);
    // creating the array of points
    $points["$left"] = "$left";
    $points["$right"] = "$right";
    // create the line and its associated points
    $LINES["$label"] = $points;
$TYPE[$label]="LINE";
}

/*
 * Add Circle - YH
 */
function add_circle($label) {  
    global $TYPE;
//    print "CREATE CIRCLE ".$label."\n";
    $TYPE[$label]="CIRCLE";
}

/*
 * Add Circle - center and point - PQ
 *
 * -> label,center and point beloging to the circle
 * <- added the two points to the list of points
 * <- added the circle with the center atribute and a list of (one)
 * points
 */
function addCircleCP($label,$center,$point) {  
    
    global $CIRCLES;
    global $POINTS;
    global $NUMPOINTS;

    addPoint($point);
    addPoint($center);

    // New points (artificial) to the circle predicade
    $auxNP=$NUMPOINTS;
    // to add a new point as 'label'+(numpoints+1)
    $auxNP++;
    $newPointA = $point.$auxNP;
    // for now I will add the new point to the list of points
    addPoint($newPointA);
    $auxNP++;
    $newPointB = $point.$auxNP;
    // for now I will add the new point to the list of points
    addPoint($newPointB);
    
    $points = array("$point" => "$point","$newPointA" => "$newPointA","$newPointB" => "$newPointB");
    $elements = array("Center" => "$center","Points"=>$points);
    // no radius
    $CIRCLES["$label"] = $elements;
    printCircle($center,$points);
}

/*
 * circle defined by two points
 * 
 * Arc defined by two point - Ignored from the moment 
 */
function addCirclePP($label,$pointA,$pointB) {  
    
    global $CIRCLES;

    addPoint($pointA);
    addPoint($pointB);
    $points = array("$pointA" => "$pointA","$pointB" => "$pointB");
    $elements = array("Points"=>$points);
    // no radius
    $CIRCLES["$label"] = $elements;
}

/*
 * circle defined by two points and a center
 *
 * Arc defined by two point and a center - Ignored from the moment 
 *
 * -> label,center and point beloging to the circle
 * <- added the two points to the list of points
 * <- added the circle with the center atribute and a list of (one)
 * points
 */
function addCircleCPP($label,$center,$pointA,$pointB) {  
    
    global $CIRCLES;

    addPoint($pointA);
    addPoint($pointB);
    addPoint($center);
    $points = array("$pointA" => "$pointA","$pointB" => "$pointB");
    $elements = array("Center" => "$center","Points"=>$points);
    // no radius
    $CIRCLES["$label"] = $elements;
}

// circle defined by three points
function addCirclePPP($label,$pointA,$pointB,$pointC) {  
    
    global $CIRCLES;
    global $POINTS;
    global $NUMPOINTS;
    
    addPoint($pointA);
    addPoint($pointB);
    addPoint($pointC);

    // New points (artificial) to the circle predicade
    $auxNP=$NUMPOINTS;
    // to add a new point, center of the circle, as 'O'+(numpoints+1)
    $auxNP++;
    $center = "O".$auxNP;
    // for now I will add the new point to the list of points
    addPoint($center);

    // List of points
    $points = array("$pointA" => "$pointA",
                    "$pointB" => "$pointB",
                    "$pointC" => "$pointC");
    // the circle element
    $elements = array("Center"=>"$center","Points"=>$points);
    // center - invented
    // no radius
    $CIRCLES["$label"] = $elements;
    printCircle($center,$points);
}

/* circle defined by center and radius given by two points
 *
 * Arc defined by two point and a center - Ignored from the moment 
 *
 */
function addCircleCRPP($label,$center,$radius,$pointA,$pointB) {  
    
    global $CIRCLES;

   addPoint($pointA);
    addPoint($pointB);
    addPoint($pointC);
    $points = array("$pointA"=>"$pointA","$pointB"=>"$pointB");
    $elements = array("Center"=>$center,"Radius"=>$radius,"Points"=>$points);
    $CIRCLES["$label"] = $elements;
}

/* circle defined by center and radius
 *
 * - Ignored from the moment 
 *
 */
function addCircleCR($label,$center,$radius) {  
    
    global $CIRCLES;

    addPoint($center);
    $elements = array("Center"=>$center,"Radius"=>$radius);
    $CIRCLES["$label"] = $elements;
}



/*
 * Add Line Bisector -  - PQ
 *
 * -> label (of the new line), PointA, PointB (of the old line)
 * <- added the line with two new points
 * <- added atribute 'perp' with the four points.
 */
function addLineBissector($line,$pointA,$pointB) {
    global $LINES;
    global $NUMPOINTS;

//    echo "\naddLineBissector(".$line.",".$pointA.",".$pointB.")\n";
    
    $points = array();

    // creating two new points
    $auxNP=$NUMPOINTS;
    // create the label with name + num
    $auxNP++;
    $newPoint1 = $pointA.$auxNP; 
    $auxNP++;
    $newPoint2 = $pointB.$auxNP;
    
    // adding the new points to the list of points
    addPoint($newPoint1);
    addPoint($newPoint2);

    // creating the array of points
    $points["$newPoint1"] = "$newPoint1";
    $points["$newPoint2"] = "$newPoint2";
    
    // the points are associated to the line identifier
    $LINES["$line"] = $points;

    // now the predicate 'perp'
    echo "perp(".$pointA.",".$pointB.",".$newPoint1.",".$newPoint2.")\n";
}


$xpath = new DOMXpath($xml);

/*
 * CREATE ALL "Natural" POINTS, i.e. the points explicitly created
 *   in the construction
 *
 * The structure $POINTS will contain all the points.
 */
$elements=$xpath->query("//construction/element[@type='point']/@label");
// first put every point on an array
foreach ($elements as $element) {
    addPoint($element->value);
}


/*
 * CREATE ALL SEGMENTS - YH/PQ
 *
 * -> the XML GeoGebra code
 * <- added to the list of lines a new identifier and its associated points 
 * <- added the points to the list of points
 */
$elements=$xpath->query("//construction/command[@name='Segment']");
foreach ($elements as $element) {
    foreach ($element->childNodes as $sub) {
        if ($sub->nodeType==XML_ELEMENT_NODE) {
            if ($sub->nodeName=="input") {
                $input=$sub;
            }
            elseif ($sub->nodeName=="output") {
                addSegment($sub->getAttribute("a0"),$input->getAttribute("a0"),$input->getAttribute("a1")); //PQ
            }
        }
    }
} /// end CREATE ALL SEGMENTS

/*
 * CREATE ALL POLYGONS - YH/PQ
 *
 * -> the XML GeoGebra code
 * <- added to the list of lines, the lines that made the poligon and its 
 *    associated points  
 * <- added all the corresponding points to the list of points
 *
 */
$elements=$xpath->query("//construction/command[@name='Polygon']");
foreach ($elements as $element) {
    foreach ($element->childNodes as $sub) {
        if ($sub->nodeType==XML_ELEMENT_NODE) {
            if ($sub->nodeName=="input") {
                $input=$sub;
                if ($input->hasAttribute("a2")) {
                    $polygonLength=$sub->getAttribute("a2");
                }
            }
            elseif ($sub->nodeName=="output") {
                if (preg_match('/^[0-9]+$/',$polygonLength)) { //// REGULAR POLYGON
                    addSegment($sub->getAttribute("a1"),$input->getAttribute("a0"),$input->getAttribute("a1"));
                    addSegment($sub->getAttribute("a2"),$input->getAttribute("a1"),$sub->getAttribute("a".($polygonLength+1)));
                    for ($i=3;$i<$polygonLength;$i++) {
                        addSegment($sub->getAttribute("a$i"),$sub->getAttribute("a".($polygonLength+$i-2)),$sub->getAttribute("a".($polygonLength+$i-1)));
                    } /// each $i (1 .. $polygonLength)
                    addSegment($sub->getAttribute("a$polygonLength"),$sub->getAttribute("a".(2*$polygonLength-2)),$input->getAttribute("a0"));
                } /// end REGULAR POLYGON
                else { /// NOT REGULAR POLYGON
                    $inatts=$input->attributes;
                    $outatts=$sub->attributes;
                    $i=1;
                    while (($sub->hasAttribute("a".$i)) and ($i<10000)) {
                        if ($input->hasAttribute("a".$i)) {
                            addSegment($sub->getAttribute("a".$i),$input->getAttribute("a".($i-1)),$input->getAttribute("a".$i));
                        } else { // on a fait le cycle
                            addSegment($sub->getAttribute("a".$i),$input->getAttribute("a".($i-1)),$input->getAttribute("a0"));
                        }
                        $i++;
                    }
                } /// end NOT REGULAR POLYGON
            }
        }
    }
} /// end CREATE ALL POLYGONS

/*
 * CREATE ALL CIRCLES
 *
 * -> the XML GeoGebra code
 * <- If it is a circle defined by three points, then we have "circle(A,B,C) 
 *    and cyclic(A,B,C) 
 * 
 * circle defined by three points
 * circle defined by center and radius
 * circle defined by center and radius given by two points
 * circle defined by center and point
 *
 */
$elements=$xpath->query("//construction/command[@name='Circle']");
foreach ($elements as $element) {
    foreach ($element->childNodes as $sub) {
        if ($sub->nodeType==XML_ELEMENT_NODE) {
            if ($sub->nodeName=="input") {
                $input=$sub;
            }
            if ($sub->nodeName=="output") {
                if ($input->hasAttribute("a2")) { // circle defined by three points
                    addCirclePPP($sub->getAttribute("a0"),$input->getAttribute("a0"),$input->getAttribute("a1"),$input->getAttribute("a2"));
                    add_circle($sub->getAttribute("a0"));
                    add_relation("POINT",$input->getAttribute("a0"),"BELONGS_TO","CIRCLE",$sub->getAttribute("a0"));
                    add_relation("POINT",$input->getAttribute("a1"),"BELONGS_TO","CIRCLE",$sub->getAttribute("a0"));
                    add_relation("POINT",$input->getAttribute("a2"),"BELONGS_TO","CIRCLE",$sub->getAttribute("a0"));
                } // end circle defined by three points
                elseif (preg_match('/^[0-9]+$/',$input->getAttribute("a1"))) {
                    // circle defined by center and radius
                    add_circle($sub->getAttribute("a0"));
                    add_relation("POINT",$input->getAttribute("a0"),"IS_CENTER_OF","CIRCLE",$sub->getAttribute("a0"));
                } // end circle defined by center and radius
                elseif (preg_match('/^Segment/',$input->getAttribute("a1"))) { // circle defined by center and radius given by two points
                    add_circle($sub->getAttribute("a0"));
                    add_relation("POINT",$input->getAttribute("a0"),"IS_CENTER_OF","CIRCLE",$sub->getAttribute("a0"));
                } /// end circle defined by center and radius given by two points
                else { // circle defined by center and point
                    addCircleCP($sub->getAttribute("a0"),$input->getAttribute("a0"),$input->getAttribute("a1"));
                    add_circle($sub->getAttribute("a0"));
                    add_relation("POINT",$input->getAttribute("a0"),"IS_CENTER_OF","CIRCLE",$sub->getAttribute("a0"));
                    add_relation("POINT",$input->getAttribute("a1"),"BELONGS_TO","CIRCLE",$sub->getAttribute("a0"));
                } // end circle defined by center and point
            }
        }
    }
} // end CREATE ALL CIRCLES

/*
 * CREATE ALL SEMICIRCLES, etc.
 *
 * two points
 * center and two points
 * sector with center and two points
 * three points
 * sector with three points
 */
// First: the "two points" cases
$elements=$xpath->query("//construction/command[@name='Semicircle' or @name='CircumcircleArc' or @name='CircumcircleSector']");
foreach ($elements as $element) {
    foreach ($element->childNodes as $sub) {
        if ($sub->nodeType==XML_ELEMENT_NODE) {
            if ($sub->nodeName=="input") {
                $input=$sub;
            }
            if ($sub->nodeName=="output") {
                add_circle($sub->getAttribute("a0"));
                add_relation("POINT",$input->getAttribute("a0"),"BELONGS_TO","CIRCLE",$sub->getAttribute("a0"));
                add_relation("POINT",$input->getAttribute("a1"),"BELONGS_TO","CIRCLE",$sub->getAttribute("a0"));
                if ($input->hasAttribute("a2")) {
                    // two points and a center
                    addCircleCPP($sub->getAttribute("a0"),$input->getAttribute("a2"),$input->getAttribute("a0"),$input->getAttribute("a1"));
                    add_relation("POINT",$input->getAttribute("a2"),"BELONGS_TO","CIRCLE",$sub->getAttribute("a0"));
                }
                else {
                    addCirclePP($sub->getAttribute("a0"),$input->getAttribute("a0"),$input->getAttribute("a1"));
                    // two point only
                    
                }
            }
        }
    }
}
// Second: the three points cases.
$elements=$xpath->query("//construction/command[@name='CircleArc' or @name='CircleSector']");
foreach ($elements as $element) {
    foreach ($element->childNodes as $sub) {
        if ($sub->nodeType==XML_ELEMENT_NODE) {
            if ($sub->nodeName=="input") {
                $input=$sub;
            }
            if ($sub->nodeName=="output") {
                add_circle($sub->getAttribute("a0"));
                add_relation("POINT",$input->getAttribute("a0"),"IS_CENTER_OF","CIRCLE",$sub->getAttribute("a0"));
                add_relation("POINT",$input->getAttribute("a1"),"BELONGS_TO","CIRCLE",$sub->getAttribute("a0"));
                add_relation("POINT",$input->getAttribute("a2"),"BELONGS_TO","CIRCLE",$sub->getAttribute("a0"));
                addCirclePPP($sub->getAttribute("a0"),$input->getAttribute("a0"),$input->getAttribute("a1"),$input->getAttribute("a2"));
            }
        }
    }
} // end CREATE ALL SEMICIRCLES, etc.


/*
 * CREATE ALL LINES
 *
 * -> the XML GeoGebra code
 * <- added to the list of lines a new identifier and its associated points 
 * <- added the points to the list of points
 */
$elements=$xpath->query("//construction/command[@name='Line' or @name='Ray']");
foreach ($elements as $element) {
    foreach ($element->childNodes as $sub) {
        if ($sub->nodeType==XML_ELEMENT_NODE) {
            if ($sub->nodeName=="input") {
                $input=$sub;
            }
            if ($sub->nodeName=="output") {
                add_line($sub->getAttribute("a0"));
                if ($TYPE[$input->getAttribute("a1")]=="POINT") {
                    add_relation("POINT",$input->getAttribute("a0"),"BELONGS_TO","LINE",$sub->getAttribute("a0"));
                    add_relation("POINT",$input->getAttribute("a1"),"BELONGS_TO","LINE",$sub->getAttribute("a0"));
                    $seg=$input->getAttribute("a0").":".$input->getAttribute("a1");
                    // PQ
                    addLine($sub->getAttribute("a0"),$input->getAttribute("a0"),$input->getAttribute("a1"));
                    if (in_array($seg,$IS_IN_FACT)) {
                        // PQ ???
                        $seg=$IS_IN_FACT[$seg];
                    }
                    else {
                        // PQ ??? não estou a considerar esta relação
                        add_segment($seg,$input->getAttribute("a0"),$input->getAttribute("a1"));
                    }
                    // PQ ???
                    add_relation("SEGMENT",$seg,"BELONGS_TO","LINE",$sub->getAttribute("a0"));
                }
                elseif ($TYPE[$input->getAttribute("a1")]=="LINE" || $TYPE[$input->getAttribute("a1")]=="SEGMENT") {
                    // line is parallel to other line                    
                    add_relation("POINT",$input->getAttribute("a0"),"BELONGS_TO","LINE",$sub->getAttribute("a0"));
                    add_relation("LINE",$sub->getAttribute("a0"),"IS_PARALLEL_TO","LINE",$input->getAttribute("a1"));
                    add_relation("LINE",$input->getAttribute("a1"),"IS_PARALLEL_TO","LINE",$sub->getAttribute("a0"));
                    // PQ create the new line with one point
                    // (the other will be created by the function)
                    // (point,line)
                    defineParallelLine($input->getAttribute("a0"),$sub->getAttribute("a0"));
                    // adding Parallel lines
                    // (line1,line2)
                    addParallelLines($sub->getAttribute("a0"),$input->getAttribute("a1"));
                }
            }
        }
    }
} // end CREATE ALL LINES


/*
 *
 * CREATE ALL ORTHOGONAL LINES
 *
 */
$elements=$xpath->query("//construction/command[@name='OrthogonalLine']");
foreach ($elements as $element) {
    foreach ($element->childNodes as $sub) {
        if ($sub->nodeType==XML_ELEMENT_NODE) {
            if ($sub->nodeName=="input") {
                $input=$sub;
            }
            if ($sub->nodeName=="output") {
                // get the name of the new line
                add_line($sub->getAttribute("a0"));
                // add two points to the new line, the one of the command and another random
                defineOrthogonalLine($input->getAttribute("a0"),$sub->getAttribute("a0"));
                addOrthogonalLine($sub->getAttribute("a0"),$input->getAttribute("a1"));
//                add_relation("LINE",$input->getAttribute("a1"),"IS_PERP_TO","LINE",$sub->getAttribute("a0"));
            }
        }
    }
} // end CREATE ALL ORTHOGONAL LINES

/*
 * CREATE ALL LINE BISECTORS
 *
 * XML Input - two points
 * XML Output - one line
 *
 * |Z| - there is no connection between the segment privously defined
 *       and the new segment... is this important?
 *
 * -> two points and a line name 
 * <-- a new line defined by two new points
 * <-- a new line defined by the old
 * <-- 'perp' predicate between the four points 
 */
$elements=$xpath->query("//construction/command[@name='LineBisector']");
foreach ($elements as $element) {
    foreach ($element->childNodes as $sub) {
        if ($sub->nodeType==XML_ELEMENT_NODE) {
            if ($sub->nodeName=="input") {
                $input=$sub; // the two points
            }
            if ($sub->nodeName=="output") {
                add_line($sub->getAttribute("a0"));
                if ($input->hasAttribute("a1")) {
                    addLineBissector($sub->getAttribute("a0"),$input->getAttribute("a0"),$input->getAttribute("a1"));
                    $seg=$input->getAttribute("a0").":".$input->getAttribute("a1");
                    if (in_array($seg,$IS_IN_FACT)) { $seg=$IS_IN_FACT[$seg]; }
                    add_relation("LINE",$sub->getAttribute("a0"),"IS_PERP_TO","SEGMENT",$seg);
                } else {
                    add_relation("LINE",$sub->getAttribute("a0"),"IS_PERP_TO","SEGMENT",$input->getAttribute("a0"));
                    // (line,point,point)
                }
            }
        }
    }
} // end CREATE ALL LINE BISECTORS


// ==================================

/*
 * CREATE ALL INTERSECTIONS
 */
$elements=$xpath->query("//construction/command[@name='Intersect']");
foreach ($elements as $element) {
    foreach ($element->childNodes as $sub) {
        if ($sub->nodeType==XML_ELEMENT_NODE) {
            if ($sub->nodeName=="input") {
                $input=$sub;
            }
            if ($sub->nodeName=="output") {
                add_relation("POINT",$sub->getAttribute("a0"),"BELONGS_TO intersec","LINE",$input->getAttribute("a0"));
                add_relation("POINT",$sub->getAttribute("a0"),"BELONGS_TO intersec","LINE",$input->getAttribute("a1"));
                // adding a point to two existing lines
                // (line1,line2,point)
                addIntersection($input->getAttribute("a0"),$input->getAttribute("a1"),$sub->getAttribute("a0"));
            }
        }
    }
} // end CREATE ALL INTERSECTIONS

/*
 * CREATE ALL MIDPOINTS
 *
 * For the moment I just add the point to the line, the questions of
 * ratios ...
 */
$elements=$xpath->query("//construction/command[@name='Midpoint']");
foreach ($elements as $element) {
    foreach ($element->childNodes as $sub) {
        if ($sub->nodeType==XML_ELEMENT_NODE) {
            if ($sub->nodeName=="input") {
                $input=$sub;
            }
            if ($sub->nodeName=="output") {
                if ($input->hasAttribute("a1")) {
                    $seg=$input->getAttribute("a0").":".$input->getAttribute("a1");
                    if (in_array($seg,$IS_IN_FACT)) { $seg=$IS_IN_FACT[$seg]; }
                    else { add_segment($seg,$input->getAttribute("a0"),$input->getAttribute("a1")); }
                } else {
                    $seg=$input->getAttribute("a0");
                }
                add_relation("POINT",$sub->getAttribute("a0"),"BELONGS_TO","SEGMENT",$seg);
                add_relation("POINT",$sub->getAttribute("a0"),"IS_CENTER_OF","SEGMENT",$seg);
                // (pointM,pointA,pointB)
                addMidpoint($sub->getAttribute("a0"),$input->getAttribute("a0"));
            }
        }
    }
} // end CREATE ALL INTERSECTIONS, MIDPOINTS


/*
 * Insert point in Object
 *
 * XML (input) --- label of object
 * XML (output) --- label of point
 *
 * Check the cases where a point belongs to an existing element
 */
$elements=$xpath->query("//construction/command[@name='Point']");
foreach ($elements as $element) {
    foreach ($element->childNodes as $sub) {
        if ($sub->nodeType==XML_ELEMENT_NODE) {
            if ($sub->nodeName=="input") {
                $input=$sub;
            }
            elseif ($sub->nodeName=="output") {
                // add point to line (point,object[line|circle])
                addPointOnObject($sub->getAttribute("a0"),$input->getAttribute("a0"),$input->getAttribute("a1")); //PQ
            }
        }
    }
} // end of: Insert point in Object


/* addCircleCP("c","O","A"); */
/* var_dump($CIRCLES); */


/* addCirclePPP("c","A","B","C"); */
/* var_dump($CIRCLES); */

/* addCircleCRPP("c","O",23,"A","B"); */
/* var_dump($CIRCLES); */

/* addCircleCR("c","O",34); */


// Create the "point" predicate
echo "points(";
$auxNP=0;
foreach ($POINTS as $value) {
    echo "$value";
    if ($auxNP < $NUMPOINTS-1) {
        echo ",";
    }
    else {
        echo ")\n";
    }
    $auxNP++;
}


// Function by Kemal Dağ in the http://stackoverflow.com forum
function getCombinations($base,$n){

    $baselen = count($base);
    if($baselen == 0){
        return;
    }
    if($n == 1){
        $return = array();
        foreach($base as $b){
            $return[] = array($b);
        }
        return $return;
    }
    else{
        //get one level lower combinations
        $oneLevelLower = getCombinations($base,$n-1);
        
        //for every one level lower combinations add one element to
        //them that the last element of a combination is preceeded by
        //the element which follows it in base array if there is none,
        //does not add
        $newCombs = array();
        
        foreach($oneLevelLower as $oll){
            
            $lastEl = $oll[$n-2];
            $found = false;
            foreach($base as  $key => $b){
                if($b == $lastEl){
                    $found = true;
                    continue;
                    //last element found
                }
                if($found == true){
                    //add to combinations with last element
                    if($key < $baselen){
                        $tmp = $oll;
                        $newCombination = array_slice($tmp,0);
                        $newCombination[]=$b;
                        $newCombs[] = array_slice($newCombination,0);
                    }
                }
            }
        }
    }
    return $newCombs;
}

//var_dump(getCombinations(array("A","B","E","C","X","Y"),3));


// Create the 'circle' predicate

function printCircle($center,$points) {

    // prints the center
    echo "circle(".$center.",";

    // prints the other points
    $auxNPoints = count($points);
    foreach ($points as $valuePoints) {
        if ($auxNPoints > 1) {
            echo $valuePoints.",";
        }
        elseif ($auxNPoints == 1) {
            echo $valuePoints;
        }  
        $auxNPoints--;              
    }
    echo ")\n";
}

// Create the 'coll' predicate
foreach ($LINES as $key => $value) {
    $nPointsInLine = count($value);
    // we do not consider the case of only two points
    if ($nPointsInLine > 2) {
        // get all the combinations of n points 3 at a time
        $combinations = getCombinations($value,3);
//        var_dump($combinations);
        // echoes the atributes
        foreach ($combinations as $arrayOfPoints) {
//            var_dump($arrayOfPoints);
            $auxNPoints = 0;
            echo "coll(";
            foreach ($arrayOfPoints as $valuePoints) {
                if ($auxNPoints <= 1) {
                    echo $valuePoints.",";
                }
                elseif ($auxNPoints == 2) {
                    echo $valuePoints;
                }  
                $auxNPoints++;              
            }
            echo ")\n";
        }
    }
}


function checkIt($whatever,$label) {
    echo $label."--->\n";
    var_dump($whatever);
}


//echo "\n\nanother test\n\n"; 


/*
echo "\nCircles\n\n";
var_dump($CIRCLES);
*/
/*
echo "\nLines\n\n";
var_dump($LINES);
*/

/*
echo "\nPoints\n\n";
var_dump($POINTS);
*/

/*
echo "\nTypes\n\n";
var_dump($TYPE);
*/

/* $l3 = array( */
/*     "A" => "A", */
/*     "B" => "B", */
/* ); */
/* var_dump($l3); */
/* $l2 = array( */
/*     "Center" => "O", */
/*     "Radius" => "23", */
/*     "Points" => $l3); */
/* var_dump($l2); */
/* $l1 = array( */
/*      "c" => $l2); */
/* var_dump($l1); */


// write the buffer to a file
/*
$chenPredicates = ob_get_contents();

fwrite($outputFile, $chenPredicates);
fclose($outputFile);

ob_end_clean();
*/

?>