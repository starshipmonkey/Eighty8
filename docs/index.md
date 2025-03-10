---
layout: default
title: Eighty8 Robotics Project
description: An innovative robotics platform for education, research, and enthusiasts
---

# Welcome to Eighty8 Robotics

## About the Project

Eighty8 is an open-source robotics platform designed to make advanced robotics accessible to educators, researchers, students, and hobbyists. Our mission is to lower the barriers to entry in robotics by providing a flexible, modular system that can be customized for various applications while remaining affordable and user-friendly.

Whether you're a beginner looking to learn the basics of robotics or an experienced engineer working on cutting-edge research, Eighty8 provides the hardware and software foundation you need to bring your ideas to life.

![Eighty8 Robot]({{ '/assets/images/robot-hero.jpg' | relative_url }})

## Key Features

### Modular Design
- **Expandable Hardware**: Easily add sensors, actuators, and custom components
- **Interchangeable Modules**: Swap parts without redesigning the entire system
- **Open Standards**: Compatible with industry-standard interfaces

### Powerful Software
- **Intuitive Programming Interface**: Control your robot with simple commands
- **Advanced Capabilities**: Access to computer vision, machine learning, and autonomous navigation
- **Cross-Platform Support**: Works with Windows, Linux, and macOS

### Community-Focused
- **Open Source**: All designs and code freely available on GitHub
- **Active Community**: Join a growing network of builders and creators
- **Comprehensive Documentation**: Tutorials, guides, and API references

## Latest Updates

{% for post in site.posts limit:3 %}
<div class="post-preview">
  <h3><a href="{{ post.url | relative_url }}">{{ post.title }}</a></h3>
  <p class="post-meta">{{ post.date | date: "%B %-d, %Y" }}</p>
  <p>{{ post.excerpt }}</p>
  <a href="{{ post.url | relative_url }}" class="read-more">Read More →</a>
</div>
{% endfor %}

[View All Updates →]({{ '/blog/' | relative_url }})

## Get Involved

Ready to start your robotics journey with Eighty8? Here's how you can get involved:

1. **Explore the Code**: [Visit our GitHub repository](https://github.com/starshipmonkey/Eighty8)
2. **Build Your Own**: Check out our [assembly guides]({{ '/docs/getting-started/' | relative_url }})
3. **Join the Community**: Participate in [discussions]({{ '/community/' | relative_url }}) and share your projects
4. **Contribute**: Help improve the platform by submitting pull requests or reporting issues

## Contact Us

Have questions or suggestions? We'd love to hear from you!

- **Email**: [info@eighty8robotics.com](mailto:info@eighty8robotics.com)
- **Twitter**: [@Eighty8Robotics](https://twitter.com/Eighty8Robotics)
- **Discord**: [Join our server](https://discord.gg/eighty8robotics)

---

<div class="cta-container">
  <h2>Ready to build something amazing?</h2>
  <a href="{{ '/docs/getting-started/' | relative_url }}" class="cta-button">Get Started Now</a>
</div>

