
#include <stdio.h>
#include <qfile.h>

#include "UmlCom.h"
#include "Dialog.h"

#include "UmlPackage.h"
#include "FileOut.h"
#include "UmlItem.h"

void UmlPackage::xmi(int argc, char ** argv) {
  if (parent() != 0) {
    UmlItem::xmi(argc, argv);
    return;
  }

  // the project

  bool noarg = (argc == 0);
  QCString path;
  QCString encoding;
  QCString genviews;
  QCString uml20;
  QCString pkprefix;
  QCString visprefix;
  QCString primitivetype;
  QCString genextension;
  QCString geneclipse;
  QCString commentexporter;
  QCString linefeed;
  
  bool comment_exporter = FALSE;  

  if (noarg) {
    propertyValue("gxmi path", path);
    propertyValue("gxmi gen views", genviews);
    propertyValue("gxmi encoding", encoding);
    propertyValue("gxmi uml2.0", uml20);
    propertyValue("gxmi pk prefix", pkprefix);
    propertyValue("gxmi vis prefix", visprefix);
    propertyValue("gxmi primitive type", primitivetype);
    propertyValue("gxmi gen extensions", genextension);
    propertyValue("gxmi gen eclipse", geneclipse);
    propertyValue("gxmi comment exporter", commentexporter);
    propertyValue("gxmi linefeed", linefeed);
    
    Dialog dialog(path, encoding, genviews, uml20, pkprefix, visprefix,
		  primitivetype, genextension, geneclipse, commentexporter, linefeed, _lang);
    
    if (dialog.exec() != QDialog::Accepted)
      return;
  }
  else if (argc >= 3) {	// at least language path and encoding
    if (!strcmp(argv[0], "-uml"))
      _lang = Uml;
    else if (!strcmp(argv[0], "-c++"))
      _lang = Cpp;
    else if (!strcmp(argv[0], "-java"))
      _lang = Java;
    else {
      UmlCom::trace("language is missing");
      return;
    }
    path = argv[1];
    encoding = argv[2];
    argc -= 3;
    argv += 3;
    
    _uml_20 = FALSE;
    _gen_views = FALSE;
    _pk_prefix = FALSE;
    _vis_prefix = FALSE;
    _primitive_type = FALSE;
    _gen_extension = FALSE;
    _gen_eclipse = FALSE;
    _linefeed = FALSE;
    
    while (argc != 0) {
      if (!strcmp(argv[0], "-view"))
	_gen_views = TRUE;
      else if (! strcmp(argv[0], "-uml2.0"))
	_uml_20 = TRUE;
      else if (! strcmp(argv[0], "-pk"))
	_pk_prefix = TRUE;
      else if (! strcmp(argv[0], "-vis"))
	_vis_prefix = TRUE;
      else if (! strcmp(argv[0], "-primitypeType"))
	_primitive_type = TRUE;
      else if (! strcmp(argv[0], "-extension"))
	_gen_extension = TRUE;
      else if (! strcmp(argv[0], "-eclipse"))
	_gen_eclipse = TRUE;
      else if (! strcmp(argv[0], "-commentexporter"))
	comment_exporter = TRUE;
      else if (! strcmp(argv[0], "-lf"))
	_linefeed = TRUE;
      else {
	UmlCom::trace("wrong parameters");
	return;
      }
      argc -= 1;
      argv += 1;
    }
  }
  else {
    UmlCom::trace("wrong number of parameters");
    return;
  }
  

  if (! path.isEmpty()) {
    // note : QTextStream(FILE *) bugged under windows
    QFile fp(path);
    
    if (! fp.open(IO_WriteOnly))
      UmlCom::trace("cannot open " + path);
    else {
      if (noarg) {
	set_PropertyValue("gxmi path", path);
	set_PropertyValue("gxmi gen views", genviews);
	set_PropertyValue("gxmi encoding", encoding);
	set_PropertyValue("gxmi uml2.0", uml20);
	set_PropertyValue("gxmi pk prefix", pkprefix);
	set_PropertyValue("gxmi vis prefix", visprefix);
	set_PropertyValue("gxmi primitive type", primitivetype);
	set_PropertyValue("gxmi gen extensions", genextension);
	set_PropertyValue("gxmi gen eclipse", geneclipse);
	set_PropertyValue("gxmi comment exporter", commentexporter);
	set_PropertyValue("gxmi linefeed", linefeed);
	_gen_views = (genviews == "yes");
	_uml_20 = (uml20 == "yes");
	_pk_prefix = (pkprefix == "yes");
	_vis_prefix = (visprefix == "yes");
	_primitive_type = (primitivetype == "yes");
	_gen_extension = (genextension == "yes");
	_gen_eclipse = (geneclipse == "yes");
	comment_exporter = (commentexporter == "yes");
	_linefeed = (linefeed == "yes");
      }
	
      {
	bool utf8;
	
	if (encoding.left(3).lower() == "utf") {
	  if (encoding.right(1) != "8") {
	    encoding = "UTF-8";
	    UmlCom::trace("warning : replace encoding by UTF-8");
	  }
	  utf8 = TRUE;
	}
	else
	  utf8 = FALSE;
	
	FileOut out(&fp, _linefeed, utf8);
	QList<UmlPackage> profiles;
	UmlPackage * prof;
	
	search_profiles(profiles);
	
	out << "\
<?xml version=\"1.0\" encoding=\"" << encoding << "\"?>\n\
<xmi:XMI xmi:version=\"2." << ((_uml_20) ? "0" : "1")
  << "\" xmlns:uml=\"http://schema.omg.org/spec/UML/2." << ((_uml_20) ? "0" : "1")
  << "\" xmlns:xmi=\"http://schema.omg.org/spec/XMI/2." << ((_uml_20) ? "0" : "1")
  << "\"";
	
	for (prof = profiles.first(); prof != 0; prof = profiles.next()) {
	  out << " xmlns:";
	  out.quote(prof->name());
	  out << "=\"http:///schemas/";
	  out.quote(prof->name());
	  out << "/1\"";
	}
	
	out << ">\n\t";
	if (comment_exporter)
	  out << "<!-- ";
	out << "<xmi:Documentation exporter=\"Bouml\" exporterVersion=\"1.5.4\"/>";
	if (comment_exporter)
	  out << " -->";
	out << "\n\t<uml:Model xmi:type=\"uml:Model\" xmi:id=\"themodel\" name=\""
	  << name() << "\">\n";
	
	for (prof = profiles.first(); prof != 0; prof = profiles.next()) {
	  out << "\t\t<profileApplication xmi:type=\"uml:ProfileApplication\"";
	  out.id_prefix(prof, "PRFA_");
	  out << ">\n\t\t\t<appliedProfile";
	  out.idref(prof);
	  out << "/>\n\t\t</profileApplication>\n";
	}
  
	out.indent(+2);
	search_class_assoc();
	write(out);
		
	out.define_datatypes(_uml_20, _primitive_type, _gen_extension);
	
	out << "\t</uml:Model>\n";
	
	write_stereotyped(out);

	out << "</xmi:XMI>\n";
      }
      
      UmlCom::trace("xmi generation done in <i>" + path + "</i><br>");
      
      fp.flush();
      fp.close();
    }
  }
}

void UmlPackage::write(FileOut & out) {
  const char * k = (_uml_20) ? "ownedMember" : "packagedElement";
  bool is_profile = (stereotype() == "profile");
  
  out.indent(); 
  out << '<' << k << " xmi:type=\"uml:"
    << ((is_profile) ? "Profile\"" : "Package\""); 
  out.id(this); 
  out << " name =\"";
  out.quote(name());
  out << '"'; 
  
  QCString mcr;
  QCString mmr;
  
  if (is_profile) {
    propertyValue("metaclassreference", mcr);
    propertyValue("metamodelReference", mmr);
    
    if (!mcr.isEmpty())
      out.ref(this, "metaclassReference", "MCR_");
    else if (!mmr.isEmpty())
      out.ref(this, "metamodelReference", "MMR_");
  }
  out << ">\n"; 
  out.indent(+1); 
  write_description_properties(out);
  
  if (!mcr.isEmpty()) {
    out.indent(); 
    out << "<elementImport";
    out.id_prefix(this, "MCR_");
    
    int index = mcr.findRev('#');
    
    if (index != -1)
      out << " alias=\"" << mcr.mid(index + 1) << "\">\n";
    else
      out << ">\n";
    
    out.indent(); 
    out << "\t<importedElement xmi:type=\"uml:Class\" href=\"" << mcr << "#1\"/>\n";
    out.indent(); 
    out << "</elementImport>\n";
  }

  if (!mmr.isEmpty()) {
    out.indent(); 
    out << "<packageImport xmi:type=\"uml:PackageImport\"";
    out.id_prefix(this, "MMR_");
    out << ">\n";
    
    out.indent(); 
    out << "\t<importedPackage xmi:type=\"uml:Model\" href=\"" << mmr << "#1\"/>\n";
    out.indent(); 
    out << "</packageImport>\n";
  }

  const QVector<UmlItem> ch = children();
  unsigned n = ch.size();
  
  for (unsigned i = 0; i != n; i += 1)
    ch[i]->write(out);
    
  while (! _relations.isEmpty())
    _relations.take(0)->write(out, FALSE);

  out.indent(-1); 
  out.indent(); 
  out << "</" << k << ">\n";
}

void UmlPackage::search_profiles(QList<UmlPackage> & l) {
  if (stereotype() == "profile")
    l.append(this);
    
  const QVector<UmlItem> ch = children();
  unsigned n = ch.size();
  
  for (unsigned i = 0; i != n; i += 1)
    if (ch[i]->kind() == aPackage)
      ((UmlPackage *) ch[i])->search_profiles(l);
}

void UmlPackage::search_class_assoc() {
  const QVector<UmlItem> ch = children();
  unsigned n = ch.size();
  
  for (unsigned i = 0; i != n; i += 1)
    ch[i]->search_class_assoc();
}

void UmlPackage::memo_relation(UmlItem * r) {
  _relations.append(r);
}

