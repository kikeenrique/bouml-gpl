
#include "UmlOperation.h"
#include "FileOut.h"

#include <stdlib.h>

#include "UmlClass.h"
#include "CppSettings.h"
#include "JavaSettings.h"
void UmlOperation::write(FileOut & out) {
  QCString decl;

  switch (_lang) {
  case Uml:
    out.indent();
    out << "<ownedOperation xmi:type=\"uml:Operation\" name=\"";
    out.quote(name());
    break;
  case Cpp:
    decl = cppDecl();
    
    if (decl.isEmpty())
      return;
    remove_comments(decl);
    out.indent();
    out << "<ownedOperation xmi:type=\"uml:Operation\" name=\"";
    out.quote(true_name(name(), cppDecl()));
    break;
  default: // Java
    decl = javaDecl();
    
    if (decl.isEmpty())
      return;
    remove_comments(decl);    
    out.indent();
    out << "<ownedOperation xmi:type=\"uml:Operation\" name=\"";
    out.quote(true_name(name(), javaDecl()));
    break;
  }
  out << '"';
  out.id(this);
  
  write_visibility(out);
  write_scope(out);
  if ((_lang == Cpp) && isCppConst())
    out << " isQuery=\"true\"";
  out << " isAbstract=\""
      << ((isAbstract()) ? "true" : "false")
      << "\">\n";
  
  out.indent(+1);

  write_annotation(out);
  write_description_properties(out);
  write_exceptions(out);
  write_return_type(out, decl);

  if (_lang == Uml)
    write_uml_params(out);
  else
    write_cpp_java_params(out, decl);

  out.indent(-1);
  out.indent();
  out << "</ownedOperation>\n";

  unload();
}

void UmlOperation::write_exceptions(FileOut & out) {
  const QValueList<UmlTypeSpec> excpts = exceptions();
  QValueList<UmlTypeSpec>::ConstIterator iter;
  
  for (iter = excpts.begin(); iter != excpts.end(); ++iter) {
    const UmlTypeSpec & e = *iter;
  
    if (e.type != 0) {
      out.indent();
      out << "<raisedException";
      out.idref(e.type);
      out << "/>\n";
    }
    else if (!e.explicit_type.isEmpty()) {
      out.indent();
      out << "<raisedException";
      out.idref_datatype(e.explicit_type);
      out << "/>\n";
    }
  }
}

void UmlOperation::write_return_type(FileOut & out, QCString decl) {
  const UmlTypeSpec & t = returnType();
  static int return_rank = 0;
  
  if ((t.type != 0) || !t.explicit_type.isEmpty()) {
    out.indent();
    out << "<ownedParameter name=\"return\" xmi:id=\"BOUML_return_"
        << ++return_rank << "\" direction=\"";
    if (_pk_prefix)
      out << "pk_";
    out << "return\">\n";

    out.indent();
    out << "\t<type";
    switch (_lang) {
    case Uml:
      if (t.type != 0)
	out.idref(t.type);
      else
	out.idref_datatype(t.explicit_type);
      break;
    case Cpp:
      write_cpp_returntype(out, decl);
      break;
    default: // java
      write_java_returntype(out, decl);
    }
    out << "/>\n";

    out.indent();
    out << "</ownedParameter>\n";
  }
}

void UmlOperation::write_cpp_returntype(FileOut & out, QCString decl) {
  // doesn't manage function pointer
  // manage keywords
  int index;
  
  if ((index = decl.find("${static}")) != -1)
    decl.remove(index, 9);
  
  if ((index = decl.find("${friend}")) != -1)
    decl.remove(index, 9);
  
  if ((index = decl.find("${virtual}")) != -1)
    decl.remove(index, 10);
  
  if ((index = decl.find("${inline}")) != -1)
    decl.remove(index, 9);
  
  if ((index = decl.find("${(}")) == -1)
    decl = "${type} ${name}";
  else
    decl.truncate(index);
  
  UmlTypeSpec t = returnType();
  
  if ((t.type != 0) ||
      !(t.explicit_type = CppSettings::type(t.explicit_type)).isEmpty())
    write_type(out, t, decl, "${name}", "${type}");
}

void UmlOperation::write_java_returntype(FileOut & out, QCString decl) {
// manage keywords
int index;

if ((index = decl.find("${visibility}")) != -1)
  decl.remove(index, 13);

if ((index = decl.find("${final}")) != -1)
  decl.remove(index, 8);

if ((index = decl.find("${static}")) != -1)
  decl.remove(index, 9);

if ((index = decl.find("${abstract}")) != -1)
  decl.remove(index, 11);

if ((index = decl.find("${synchronized}")) != -1)
  decl.remove(index, 15);

if ((index = decl.find("${@}")) != -1)
  decl.remove(index, 4);

if ((index = decl.find("${(}")) == -1)
  decl = "${type} ${name}";
else
  decl.truncate(index);

UmlTypeSpec t = returnType();

if ((t.type != 0) ||
    !(t.explicit_type = JavaSettings::type(t.explicit_type)).isEmpty())
  write_type(out, t, decl, "${name}", "${type}");
}

void UmlOperation::write_uml_params(FileOut & out) {
  const QValueList<UmlParameter> p = params();
  QValueList<UmlParameter>::ConstIterator it;
  
  for (it = p.begin(); it != p.end(); ++it) {
    out.indent();
    out << "<ownedParameter xmi:type=\"uml:Parameter\" name=\"" << (*it).name
      << "\" xmi:id=\"BOUML_op_param_"
	<< ++param_id << "\" direction=\"";
    if (_pk_prefix)
      out << "pk_";
    switch ((*it).dir) {
    case InputOutputDirection: out << "inout\">\n"; break;
    case OutputDirection: out << "out\">\n"; break;
    default: out << "in\">\n";
    }
    
    out.indent(+1);
    UmlItem::write_type(out, (*it).type);    
    out.indent(-1);

    out.indent();
    out << "</ownedParameter>\n";
  }
}

void UmlOperation::write_cpp_java_params(FileOut & out, QCString decl) {
  int index1 = decl.find("${(}");
    
  if (index1 == -1)
    return;
    
  int index2 = decl.find("${)}", index1 + 4);
    
  if (index2 == -1)
    return;
    
  decl = decl.mid(index1 + 4, index2 - index1 - 4);
    
  index1 = 0;
    
  const QValueList<UmlParameter> p = params();
  QCString sparam;
  QCString kname;
  QCString ktype;
  int rank;
        
  while (get_param(decl, index1, sparam, kname, ktype, rank)) {
    if (rank < (int) p.count()) {
      const UmlParameter & pa = p[rank];
      
      out.indent();
      out << "<ownedParameter xmi:type=\"uml:Parameter\" name=\"" << pa.name
	<< "\" xmi:id=\"BOUML_op_param_"
	  << ++param_id << "\" direction =\"";
      if (_pk_prefix)
	out << "pk_";
      switch (pa.dir) {
      case InputOutputDirection: out << "inout\">\n"; break;
      case OutputDirection: out << "out\">\n"; break;
      default: out << "in\">\n";
      }
      
      UmlTypeSpec t = pa.type;
      
      if ((t.type != 0) ||
	  !(t.explicit_type = (_lang == Cpp)
	    ? CppSettings::type(t.explicit_type)
	    : JavaSettings::type(t.explicit_type)).isEmpty()) {
	out.indent();
	out << "\t<type";
	write_type(out, t, sparam, kname, ktype);
	out << "/>\n";
      }
      out.indent();
      out << "</ownedParameter>\n";
    }
  }
}

bool UmlOperation::get_param(QCString s, int & index, QCString & r, QCString & kname, QCString & ktype, int & rank) {
int index0 = index;
int level = 0;
//const char * p = (const char *) s;

r = "";

do {
  if (index >= (int) s.length()) {
    if (level != 0)
      return FALSE;
    r = s.mid(index0, index - index0).stripWhiteSpace();
    if (r.isEmpty())
      return FALSE;
    break;
  }
  switch (s[index]) {
  case ',':
    if (level == 0) {
      r = s.mid(index0, index - index0).stripWhiteSpace();
      index += 1;
      if (r.isEmpty())
	return FALSE;
    }
    break;
  case '(':
  case '{':
  case '[':
    level += 1;
    break;
  case ')':
  case '}':
  case ']':
    if (--level < 0)
      return FALSE;
  default:
    break;
  }
  
  index += 1;
} while (r.isEmpty());

int index1;
int index2;

rank = -1;

if (((index1 = r.find("${p")) != -1) &&
    ((index2 = r.find("}", index1 + 3)) != -1)) {
  kname = r.mid(index1, index2 - index1 + 1);
  rank = atoi(((const char *) r) + index1 + 3);
}
else
  kname = "";

if (((index1 = r.find("${t")) != -1) &&
    ((index2 = r.find("}", index1 + 3)) != -1)) {
  ktype = r.mid(index1, index2 - index1 + 1);
  if (rank == -1)
    rank = atoi(((const char *) r) + index1 + 3);
}
else if (rank == -1)
  // too complicated;
  return FALSE;
else
  ktype = "";

return TRUE;
}

int UmlOperation::param_id;

