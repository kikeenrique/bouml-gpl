
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
  QCString useprofile;
  QCString pkprefix;
  QCString visprefix;
  QCString primitivetype;
  QCString genextension;
  QCString linefeed;

  if (noarg) {
    propertyValue("gxmi path", path);
    propertyValue("gxmi gen views", genviews);
    propertyValue("gxmi encoding", encoding);
    propertyValue("gxmi uml2.0", uml20);
    propertyValue("gxmi use profile", useprofile);
    propertyValue("gxmi pk prefix", pkprefix);
    propertyValue("gxmi vis prefix", visprefix);
    propertyValue("gxmi primitive type", primitivetype);
    propertyValue("gxmi gen extensions", genextension);
    propertyValue("gxmi linefeed", linefeed);
    
    Dialog dialog(path, encoding, genviews, uml20, useprofile, pkprefix,
		  visprefix, primitivetype, genextension, linefeed, _lang);
    
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
    _use_profile = FALSE;
    _pk_prefix = FALSE;
    _vis_prefix = FALSE;
    _primitive_type = FALSE;
    _gen_extension = FALSE;
    _linefeed = FALSE;
    
    while (argc != 0) {
      if (!strcmp(argv[0], "-view"))
	_gen_views = TRUE;
      else if (! strcmp(argv[0], "-uml2.0"))
	_uml_20 = TRUE;
      else if (! strcmp(argv[0], "-profile"))
	_use_profile = TRUE;
      else if (! strcmp(argv[0], "-pk"))
	_pk_prefix = TRUE;
      else if (! strcmp(argv[0], "-vis"))
	_vis_prefix = TRUE;
      else if (! strcmp(argv[0], "-primitypeType"))
	_primitive_type = TRUE;
      else if (! strcmp(argv[0], "-extension"))
	_gen_extension = TRUE;
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
	set_PropertyValue("gxmi use profile", useprofile);
	set_PropertyValue("gxmi pk prefix", pkprefix);
	set_PropertyValue("gxmi vis prefix", visprefix);
	set_PropertyValue("gxmi primitive type", primitivetype);
	set_PropertyValue("gxmi gen extensions", genextension);
	set_PropertyValue("gxmi linefeed", linefeed);
	_gen_views = (genviews == "yes");
	_uml_20 = (uml20 == "yes");
	_use_profile = (useprofile == "yes");
	_pk_prefix = (pkprefix == "yes");
	_vis_prefix = (visprefix == "yes");
	_primitive_type = (primitivetype == "yes");
	_gen_extension = (genextension == "yes");
	_linefeed = (linefeed == "yes");
      }
	
      {
	FileOut out(&fp, _linefeed);
	
	// header

	out << "\
<?xml version=\"1.0\" encoding=\"" << encoding << "\"?>\n\
<xmi:XMI xmi.version=\"2.1\" xmlns:uml=\"http://schema.omg.org/spec/UML/2." << ((_uml_20) ? "0" : "1")
  << "\" xmlns:xmi=\"http://schema.omg.org/spec/XMI/2.1\"";
	if (_use_profile)
	  out << " xmlns:boumlprofile=\"http://bouml.free.fr/profiles/boumlprofile/1.0\"";
	out << ">\n\
	<xmi:Documentation exporter=\"Bouml\" exporterVersion=\"1.0\"/>\n\
	<uml:Model xmi:type=\"uml:Model\" name=\"" << name() << "\">\n";
	out.indent(+2);
	write(out);
		
	// non class types

	out.define_datatypes(_uml_20, _primitive_type, _gen_extension);

	// stereotypes
	if (_use_profile)
#ifndef WIN32
#warning a faire
#endif
	  ;

	// footer
	
	out << "\
	</uml:Model>\n\
</xmi:XMI>\n";
      }
      
      UmlCom::trace("xmi generation done in <i>" + path + "</i><br>");
      
      fp.flush();
      fp.close();
    }
  }
}

void UmlPackage::write(FileOut & out) {
  out.indent(); 
  out << ((_uml_20) ? "<ownedMember" : "<packagedElement") 
    << " xmi:type=\"uml:Package\""; 
  out.id(this); 
  out << " name =\"";
  out.quote(name());
  out << "\">\n"; 
  out.indent(+1); 
  write_description_properties(out);   

  const QVector<UmlItem> ch = children();
  unsigned n = ch.size();
  
  for (unsigned i = 0; i != n; i += 1)
    ch[i]->write(out);
    
  while (! _relations.isEmpty())
    _relations.take(0)->write(out, FALSE);

  out.indent(-1); 
  out.indent(); 
  out << ((_uml_20) ? "</ownedMember>\n" : "</packagedElement>\n");
}

void UmlPackage::memo_relation(UmlItem * r) {
  _relations.append(r);
}

