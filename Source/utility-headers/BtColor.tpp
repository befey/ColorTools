template <class Archive,
cereal::traits::EnableIf<cereal::traits::is_text_archive<Archive>::value>>
void BtColor::save( Archive & ar ) const
{
    ar(aiColor.kind,
       
       aiColor.c.g.gray,
       
       aiColor.c.f.cyan,
       aiColor.c.f.magenta,
       aiColor.c.f.yellow,
       aiColor.c.f.black,
       
       aiColor.c.rgb.red,
       aiColor.c.rgb.green,
       aiColor.c.rgb.blue,
       
       aiColor.c.c.tint,
       
       method);
}

template <class Archive,
cereal::traits::EnableIf<cereal::traits::is_text_archive<Archive>::value>>
void BtColor::load( Archive & ar )
{
    ar(aiColor.kind,
       
       aiColor.c.g.gray,
       
       aiColor.c.f.cyan,
       aiColor.c.f.magenta,
       aiColor.c.f.yellow,
       aiColor.c.f.black,
       
       aiColor.c.rgb.red,
       aiColor.c.rgb.green,
       aiColor.c.rgb.blue,
       
       aiColor.c.c.tint,
       
       method);
}
