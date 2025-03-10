---
layout: default
title: Eighty8 Project Blog
permalink: /blog/
---

# Eighty8 Project Blog

Stay updated with the latest developments, insights, and progress on the Eighty8 robotics project.

## Latest Posts

<div class="post-list">
  {% for post in paginator.posts %}
    <div class="post-item">
      <h2>
        <a href="{{ post.url | relative_url }}">{{ post.title }}</a>
      </h2>
      <div class="post-meta">
        <span class="post-date">{{ post.date | date: "%B %d, %Y" }}</span>
        {% if post.author %}
          <span class="post-author">by {{ post.author }}</span>
        {% endif %}
        {% if post.categories.size > 0 %}
          <span class="post-categories">
            in 
            {% for category in post.categories %}
              <a href="/categories/{{ category | slugify }}">{{ category }}</a>{% unless forloop.last %}, {% endunless %}
            {% endfor %}
          </span>
        {% endif %}
      </div>
      <div class="post-excerpt">
        {{ post.excerpt }}
        <a href="{{ post.url | relative_url }}" class="read-more">Read more</a>
      </div>
    </div>
  {% endfor %}
</div>

<!-- Pagination links -->
{% if paginator.total_pages > 1 %}
<div class="pagination">
  {% if paginator.previous_page %}
    <a href="{{ paginator.previous_page_path | relative_url }}" class="pagination-item">&laquo; Previous</a>
  {% else %}
    <span class="pagination-item pagination-disabled">&laquo; Previous</span>
  {% endif %}

  {% for page in (1..paginator.total_pages) %}
    {% if page == paginator.page %}
      <span class="pagination-item pagination-active">{{ page }}</span>
    {% elsif page == 1 %}
      <a href="{{ '/blog/' | relative_url }}" class="pagination-item">{{ page }}</a>
    {% else %}
      <a href="{{ site.paginate_path | relative_url | replace: ':num', page }}" class="pagination-item">{{ page }}</a>
    {% endif %}
  {% endfor %}

  {% if paginator.next_page %}
    <a href="{{ paginator.next_page_path | relative_url }}" class="pagination-item">Next &raquo;</a>
  {% else %}
    <span class="pagination-item pagination-disabled">Next &raquo;</span>
  {% endif %}
</div>
{% endif %}

