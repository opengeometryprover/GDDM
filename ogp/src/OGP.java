package ogp;

import java.util.Map;
import java.util.Set;

public class OGP {

    public static void main(String[] args) {
	OGPConf configuration = new OGPConf();
	OGPArgs arguments = new OGPArgs(args, configuration);
	if (arguments.getHelp()) {
	    helpMsg();
	} else if (arguments.getVersion()) {
	    versionMsg(configuration.getVersion());
	} else if (arguments.getProvers()) {
	    proversList(configuration);
	} else {
	    System.out.println("          Timeout: |" + arguments.getTimeout() + "|");
	    System.out.println("    Conjecture id: |" + arguments.getConjectureId() + "|");
	}
	System.exit(0);
    }
	
    private static void helpMsg() {
	System.out.println("Usage: ogp [OPTION] [CONJECTURE [PROVER [PROVER-OPTIONS]]");
	System.out.println("where OPTION is one of:");
	System.out.print("    -h | --help                     ");
	System.out.println("prints help message (to be used alone)");
	System.out.print("    -p | --provers                  ");
	System.out.println("lists avaliable provers (to be used alone)");
	System.out.print("    -V | --version                  ");
	System.out.println("print OGP version (to be used alone)");
	System.out.print("    -t <time> | --timeout=<time>    ");
	System.out.println("sets timeout for an attempt (in seconds)");
    }

    private static void versionMsg(String version) {
	System.out.println("OpenGeometryProver " + version);
	System.out.println("Copyright (C) 2021 Nuno Baeta, Pedro Quaresma");
	System.out.println("Published under GNU GPL, version 3 or later");
	System.out.println("https://github.com/opengeometryprover/OpenGeometryProver");
    }

    private static void proversList(OGPConf configuration) {
	int nrProvers = configuration.getNrProvers();
		
	int i = 1;
	for (String proverId : configuration.getProversSet()) {
	    OGPProverInfo proverIdInfo = configuration.proverInfo(proverId);

	    System.out.println("NAME");
	    System.out.println("    " + proverIdInfo.getName());

	    System.out.println("DESCRIPTION");
	    StringBuilder description = new StringBuilder();
	    description.append(proverIdInfo.getDesc());
	    int index = description.indexOf("\\n");
	    while (index != -1) {
		System.out.println("    " + description.substring(0, index));
		description.delete(0, index + 2);
		index = description.indexOf("\\n");
	    }
	    System.out.println("    " + description);
	    
	    System.out.println("OGP ID");
	    System.out.println("    " + proverId);
	    
	    System.out.println("COMMAND");
	    System.out.println("    " + proverIdInfo.getCmd());
	    
	    System.out.println("EXTENSION");
	    System.out.println("    " + proverIdInfo.getExt());
	    
	    System.out.println(proverIdInfo.getExt().toUpperCase() + "2FOF");
	    System.out.println("    " + proverIdInfo.getToFOFCmd());
	    
	    System.out.println("FOF2" + proverIdInfo.getExt().toUpperCase());
	    System.out.println("    " + proverIdInfo.getToExtCmd());
	    
	    System.out.println("POST-PROCESSING");
	    System.out.println("    " + proverIdInfo.getPostProcCmd());
	    
	    if (i != nrProvers) {
		i++;
		System.out.println();
	    }
	}
    }
    
}
