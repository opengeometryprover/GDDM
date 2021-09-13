package ogp;

import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Map;
import java.util.Set;

public class OGP {

    // Error messages identification
    private final static int ERR_INTR = 101;
    private final static int ERR_IO_PROB = 102;
    
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
	    prove(configuration, arguments);
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

    private static void prove(OGPConf configuration, OGPArgs arguments) {
	try {
	    String conjN = arguments.getConjName();
	    String conjE = arguments.getConjExt();
	    String conjId = conjN + "." + conjE;
	    String conj = conjId + "_" + arguments.getProverId();

	    ArrayList<String> cmd = new ArrayList<String>();
	    
	    // Preprocessing
	    if (!conjE.equals(configuration
			      .proverExt(arguments.getProverId()))) {
		cmd.clear();
		cmd.add(arguments.getToFOFCmd());
		// cmd.add(conjId);
		// cmd.add(">");
		// cmd.add(conjN + "." + configuration.proverExt(arguments.getProverId()));
		Process procPreProc = new ProcessBuilder(cmd).start();
		procPreProc.waitFor();
	    }

	    // Proof
	    File conjOut = new File(conj + ".out");
	    File conjErr = new File(conj + ".err");
	    cmd.clear();
	    cmd.add("timeout");
	    cmd.add(String.valueOf(arguments.getTimeout()));
	    cmd.add(configuration.proverInfo(arguments.getProverId()).getCmd());
	    if (!conjE.equals(configuration
			      .proverExt(arguments.getProverId()))) {
		cmd.add(conjN + "."
			+ configuration.proverExt(arguments.getProverId()));
	    } else {
		cmd.add(conjId);
	    }
	    if (!arguments.getProverArgs().isEmpty()) {
		cmd.add(arguments.getProverArgs());
	    }
	    long startTime = System.nanoTime();
	    Process procProof = new ProcessBuilder(cmd)
		.redirectOutput(conjOut)
		.redirectError(conjErr)
		.start();
	    procProof.waitFor();
	    long stopTime = System.nanoTime();
	    long time = stopTime - startTime;
	    FileWriter conjTime = new FileWriter(conj + ".time");
	    if (time > arguments.getTimeout()*Math.pow(10, 9)) {
		conjTime.write("Time out!\n");
	    } else {
		conjTime.write(String.valueOf(time*Math.pow(10, -9)) + "\n");

		// Postprocessing
		if (!arguments.getProverId().startsWith("ogp")) {
		    cmd.clear();
		    cmd.add(configuration.proverInfo(arguments.getProverId())
			    .getPostProcCmd());
		    cmd.add(conj);
		    Process procPostProc = new ProcessBuilder(cmd).start();
		    procPostProc.waitFor();
		}
	    }
	    conjTime.close();
	} catch (InterruptedException e) {
	    errorMsg(ERR_INTR, e.toString());
	} catch (IOException e) {
	    errorMsg(ERR_IO_PROB, e.toString());
	}
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

    private static void versionMsg(String version) {
	System.out.println("OpenGeometryProver " + version);
	System.out.println("Copyright (C) 2021 Nuno Baeta, Pedro Quaresma");
	System.out.println("Published under GNU GPL, version 3 or later");
	System.out.println("https://github.com/opengeometryprover/OpenGeometryProver");
    }

    private static void errorMsg(int error, String msg) {
	System.err.println("[OGP ERROR " + error + "] (OGP) ");
	switch (error) {
	case ERR_INTR:
	    System.err.println("Interrupted exception caught.");
	    System.err.println(msg);
	case ERR_IO_PROB:
	    System.err.println("I/O exception caught.");
	    System.err.println(msg);
	}
	System.exit(error);
    }

}
