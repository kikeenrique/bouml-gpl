
#include "UmlItem.h"
#include "FileOut.h"
#include "UmlTypeSpec.h"

#include "UmlCom.h"
#include "UmlClass.h"

#include "PackageGlobalCmd.h"
 UmlItem::~UmlItem() {
}

void UmlItem::xmi(int, char **) {
  UmlCom::trace("Error : must be applied on the project<br>");
}

void UmlItem::write(FileOut & out) {
  const QVector<UmlItem> ch = children();
  unsigned n = ch.size();
  
  for (unsigned i = 0; i != n; i += 1)
    ch[i]->write(out);

}

void UmlItem::write(FileOut &, bool) {
}

void UmlItem::write_description_properties(FileOut & out) {
  if (! description().isEmpty()) {
    static int rank = 0;
    
    out.indent();
    out << "<ownedComment xmi:type=\"uml:Comment\" xmi:id=\"COMMENT_"
      << ++rank << "\" body=\"";
    out.quote(description());
    out << "\"/>\n";
  }

  QCString ste = stereotype();
  
  if (_gen_extension) {
    const QDict<QCString> up = properties();    
    QDictIterator<QCString> it(up);
    
    if (it.current()) {
      out.indent();
      out << "<xmi:Extension extender=\"Bouml\">\n";
      
      if (! ste.isEmpty()) {
	out.indent();
	out << "\t<stereotype name=\"";
	out.quote(ste);
	out << "\"/>\n";
      }
      
      do {
	out.indent();
	out << "\t<taggedValue tag=\"";
	out.quote(it.currentKey());
	out << "\" value=\"";
	out.quote(*(it.current()));
	out << "\"/>\n";
	++it;
      } while (it.current());
      
      out.indent();
      out << "</xmi:Extension>\n";
    }
    else if (! ste.isEmpty()) {
      out.indent();
      out << "<xmi:Extension extender=\"Bouml\">\n";
      out.indent();
      out << "\t<stereotype name=\"";
      out.quote(ste);
      out << "\"/>\n";
      out.indent();
      out << "</xmi:Extension>\n";
    }
  } 
  
  if (ste.contains(':') == 1)
    // probably a stereotype part of profile
    _stereotypes[ste].append(this);
}

void UmlItem::search_class_assoc() {
}

void UmlItem::memo_relation(UmlItem * r) {
  parent()->memo_relation(r);
}

void UmlItem::write_multiplicity(FileOut & out, QCString s)
{
  if (!s.isEmpty()) {
    static UmlItem * rank = 0;
    
    QCString min;
    QCString max;
    int index = s.find("..");
    
    if (index != -1) {
      min = s.left(index).stripWhiteSpace();
      max = s.mid(index+2).stripWhiteSpace();
    }
    else
      min = max = s.stripWhiteSpace();
    
    out.indent();
    out << "<lowerValue xmi:type=\"uml:LiteralString\"";
    out.id_prefix(++rank, "MULTIPLICITY_");
    out << " value=\"" << min << "\"/>\n";
    
    out.indent();
    out << "<upperValue xmi:type=\"uml:LiteralString\"";
    out.id_prefix(++rank, "MULTIPLICITY_");
    out << " value=\"" << max << "\"/>\n";
  }
}

void UmlItem::write_type(FileOut & out, const UmlTypeSpec & t)
{
  if (t.type != 0) {
    out.indent();
    out << "<type xmi:type=\"uml:Class\"";
    out.idref(t.type);
    out << "/>\n";
  }
  else if (!t.explicit_type.isEmpty()) {
    out.indent();
    if (t.explicit_type == "int")
      out << ((_uml_20)
	      ? "<type xmi:type=\"uml:PrimitiveType\" href=\"http://schema.omg.org/spec/UML/2.0/uml.xml#Integer\"/>\n"
	      : "<type xmi:type=\"uml:PrimitiveType\" href=\"http://schema.omg.org/spec/UML/2.1/uml.xml#Integer\"/>\n");
    else if (t.explicit_type == "bool")
      out << ((_uml_20)
	      ? "<type xmi:type=\"uml:PrimitiveType\" href=\"http://schema.omg.org/spec/UML/2.0/uml.xml#Boolean\"/>\n"
	      : "<type xmi:type=\"uml:PrimitiveType\" href=\"http://schema.omg.org/spec/UML/2.1/uml.xml#Boolean\"/>\n");
    else if (t.explicit_type == "string")
      out << ((_uml_20)
	      ? "<type xmi:type=\"uml:PrimitiveType\" href=\"http://schema.omg.org/spec/UML/2.0/uml.xml#String\"/>\n"
	      : "<type xmi:type=\"uml:PrimitiveType\" href=\"http://schema.omg.org/spec/UML/2.1/uml.xml#String\"/>\n");
    else if (t.explicit_type == "long")
      out << ((_uml_20)
	      ? "<type xmi:type=\"uml:PrimitiveType\" href=\"http://schema.omg.org/spec/UML/2.0/uml.xml#UnlimitedNatural\"/>\n"
	      : "<type xmi:type=\"uml:PrimitiveType\" href=\"http://schema.omg.org/spec/UML/2.1/uml.xml#UnlimitedNatural\"/>\n");
    else {
      out << "<type xmi:type=\"uml:Class\"";
      out.idref_datatype(t.explicit_type);
      out << "/>\n";
    }
  }

}

void UmlItem::write_default_value(FileOut & out, QCString v)
{
  if (! v.isEmpty()) {
    if (v[0] == '=') {
      v = v.mid(1);
      if (v.isEmpty())
	return;
    }

    static UmlItem * rank = 0;

    out.indent();
    out << "<defaultValue xmi:type=\"uml:LiteralString\"";
    out.id_prefix(rank, "VALUE_");
    out << " value=\"";
    out.quote(v);
    out << "\"/>\n";
  }
}

void UmlItem::write_stereotyped(FileOut & out)
{
  QMap<QCString, QList<UmlItem> >::Iterator it;
  
  for (it = _stereotypes.begin(); it != _stereotypes.end(); ++it) {
#ifndef WIN32
#warning must use UmlPackage::findStereotype(itmap.key(), TRUE)
#endif
    const char * st = it.key();
    UmlCom::send_cmd(packageGlobalCmd, saveProjectCmd + 3, (void *) 1, st);
    UmlClass * cl = (UmlClass *) UmlBaseItem::read_();
		     
    if (cl != 0) {
      QList<UmlItem> & l = it.data();
      UmlItem * elt;
      
      for (elt = l.first(); elt != 0; elt = l.next()) {
	out << "\t<" << st;
	out.id_prefix(elt, "STELT_");
	
	const QDict<QCString> props = elt->properties();
	QDictIterator<QCString> itp(props);
	
	while (itp.current()) {
	  QString k = itp.currentKey();
	  
	  if (k.contains(':') == 2) {
	    out << ' ';
	    out.quote(k.mid(k.findRev(':') + 1));
	    out << "=\"";
	    out.quote(*itp.current());
	    out << '"';
	  }
	  ++itp;
	}
	
	QCString extending;
	
	cl->propertyValue("stereotypeExtension", extending);
	if (extending.isEmpty())
	  extending = "#Element";

	QCString vr = "base_" + extending.mid(extending.findRev('#') + 1);
	
	out.ref(elt, vr);
	
	out << "/>\n";
	
	elt->unload();
      }
    }
  }

}

bool UmlItem::_gen_views;

Language UmlItem::_lang;

bool UmlItem::_uml_20;

bool UmlItem::_pk_prefix;

bool UmlItem::_vis_prefix;

bool UmlItem::_primitive_type;

bool UmlItem::_linefeed;

bool UmlItem::_gen_extension;

bool UmlItem::_gen_eclipse;

QMap<QCString, QList<UmlItem> > UmlItem::_stereotypes;

